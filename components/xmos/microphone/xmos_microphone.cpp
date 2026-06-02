#include "xmos_microphone.h"

#ifdef USE_ESP32

#include "esphome/components/audio/audio.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"

namespace esphome::i2s_audio {

static const UBaseType_t MAX_LISTENERS = 16;
static const uint32_t READ_DURATION_MS = 16;
static const size_t TASK_STACK_SIZE = 4096;
static const ssize_t TASK_PRIORITY = 23;

static const char *const TAG = "i2s_audio.xmos_microphone";

enum MicrophoneEventGroupBits : uint32_t {
  COMMAND_STOP = (1 << 0),
  TASK_STARTING = (1 << 10),
  TASK_RUNNING = (1 << 11),
  TASK_STOPPED = (1 << 13),
  ALL_BITS = 0x00FFFFFF,
};

void XMOSMicrophone::setup() {
  ESP_LOGCONFIG(TAG, "Setting up XMOS I2S microphone...");

  if (this->pdm_) {
    ESP_LOGE(TAG, "PDM is not supported for XMOS microphones");
    this->mark_failed();
    return;
  }

  this->active_listeners_semaphore_ = xSemaphoreCreateCounting(MAX_LISTENERS, MAX_LISTENERS);
  if (this->active_listeners_semaphore_ == nullptr) {
    ESP_LOGE(TAG, "Creating semaphore failed");
    this->mark_failed();
    return;
  }

  this->event_group_ = xEventGroupCreate();
  if (this->event_group_ == nullptr) {
    ESP_LOGE(TAG, "Creating event group failed");
    this->mark_failed();
    return;
  }

  this->configure_voice_stream_settings_();
}

void XMOSMicrophone::dump_config() {
  ESP_LOGCONFIG(TAG,
                "XMOS Microphone:\n"
                "  DIN Pin: %d\n"
                "  Capture: 48kHz / 32-bit I2S\n"
                "  Output: 16kHz / 16-bit PCM\n"
                "  DC offset correction: %s",
                static_cast<int8_t>(this->din_pin_), YESNO(this->correct_dc_offset_));
}

void XMOSMicrophone::configure_voice_stream_settings_() {
  uint8_t channel_count = this->slot_mode_ == I2S_SLOT_MODE_STEREO ? 2 : 1;
  this->audio_stream_info_ = audio::AudioStreamInfo(16, channel_count, 16000);
}

void XMOSMicrophone::start() {
  if (this->is_failed())
    return;
  xSemaphoreTake(this->active_listeners_semaphore_, 0);
}

void XMOSMicrophone::stop() {
  if (this->state_ == microphone::STATE_STOPPED || this->is_failed())
    return;
  xSemaphoreGive(this->active_listeners_semaphore_);
}

bool XMOSMicrophone::start_driver_() {
  if (!this->parent_->try_lock()) {
    return false;
  }
  this->locked_driver_ = true;

  i2s_chan_config_t chan_cfg = {
      .id = this->parent_->get_port(),
      .role = this->i2s_role_,
      .dma_desc_num = 4,
      .dma_frame_num = 256,
      .auto_clear = false,
  };

  esp_err_t err = i2s_new_channel(&chan_cfg, nullptr, &this->rx_handle_);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error creating RX channel: %s", esp_err_to_name(err));
    return false;
  }

  i2s_clock_src_t clk_src = I2S_CLK_SRC_DEFAULT;
#ifdef I2S_CLK_SRC_APLL
  if (this->use_apll_) {
    clk_src = I2S_CLK_SRC_APLL;
  }
#endif

  i2s_std_clk_config_t clk_cfg = {
      .sample_rate_hz = this->sample_rate_,
      .clk_src = clk_src,
      .mclk_multiple = this->mclk_multiple_,
  };
  i2s_std_slot_config_t slot_cfg =
      I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG((i2s_data_bit_width_t) this->slot_bit_width_, this->slot_mode_);
  slot_cfg.slot_bit_width = this->slot_bit_width_;
  slot_cfg.slot_mask = this->std_slot_mask_;

  i2s_std_gpio_config_t pin_config = this->parent_->get_pin_config();
  pin_config.din = this->din_pin_;

  i2s_std_config_t std_cfg = {
      .clk_cfg = clk_cfg,
      .slot_cfg = slot_cfg,
      .gpio_cfg = pin_config,
  };

  err = i2s_channel_init_std_mode(this->rx_handle_, &std_cfg);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error initializing RX channel: %s", esp_err_to_name(err));
    return false;
  }

  err = i2s_channel_enable(this->rx_handle_);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error enabling RX channel: %s", esp_err_to_name(err));
    return false;
  }

  this->configure_voice_stream_settings_();
  return true;
}

void XMOSMicrophone::stop_driver_() {
  if (this->rx_handle_ != nullptr) {
    esp_err_t err = i2s_channel_disable(this->rx_handle_);
    if (err != ESP_OK) {
      ESP_LOGW(TAG, "Error disabling RX channel: %s", esp_err_to_name(err));
    }
    err = i2s_del_channel(this->rx_handle_);
    if (err != ESP_OK) {
      ESP_LOGW(TAG, "Error deleting RX channel: %s", esp_err_to_name(err));
    }
    this->rx_handle_ = nullptr;
  }
  if (this->locked_driver_) {
    this->parent_->unlock();
    this->locked_driver_ = false;
  }
}

size_t XMOSMicrophone::read_(uint8_t *buf, size_t len, TickType_t ticks_to_wait) {
  size_t bytes_read = 0;
  esp_err_t err = i2s_channel_read(this->rx_handle_, buf, len, &bytes_read, pdTICKS_TO_MS(ticks_to_wait));
  if ((err != ESP_OK) && ((err != ESP_ERR_TIMEOUT) || (ticks_to_wait != 0))) {
    if (!this->status_has_warning()) {
      ESP_LOGW(TAG, "Read error: %s", esp_err_to_name(err));
    }
    this->status_set_warning();
    return 0;
  }
  if ((bytes_read == 0) && (ticks_to_wait > 0)) {
    this->status_set_warning();
    return 0;
  }
  this->status_clear_warning();
  return bytes_read;
}

void XMOSMicrophone::fix_dc_offset_(std::vector<uint8_t> &data) {
  const uint8_t dc_filter_shift = 10;
  const size_t bytes_per_sample = this->audio_stream_info_.samples_to_bytes(1);
  const uint32_t total_samples = this->audio_stream_info_.bytes_to_samples(data.size());
  for (uint32_t sample_index = 0; sample_index < total_samples; ++sample_index) {
    const uint32_t byte_index = sample_index * bytes_per_sample;
    int32_t input = audio::unpack_audio_sample_to_q31(&data[byte_index], bytes_per_sample);
    int32_t output = input - this->dc_offset_prev_input_ +
                     (this->dc_offset_prev_output_ - (this->dc_offset_prev_output_ >> dc_filter_shift));
    this->dc_offset_prev_input_ = input;
    this->dc_offset_prev_output_ = output;
    audio::pack_q31_as_audio_sample(output, &data[byte_index], bytes_per_sample);
  }
}

void XMOSMicrophone::mic_task(void *params) {
  auto *this_microphone = static_cast<XMOSMicrophone *>(params);
  xEventGroupSetBits(this_microphone->event_group_, MicrophoneEventGroupBits::TASK_STARTING);

  {
    const uint8_t channel_count = this_microphone->slot_mode_ == I2S_SLOT_MODE_STEREO ? 2 : 1;
    const size_t capture_frames = this_microphone->sample_rate_ * READ_DURATION_MS / 1000;
    const size_t input_bytes_to_read = capture_frames * channel_count * sizeof(int32_t);
    const size_t max_output_bytes = (capture_frames / 3) * channel_count * sizeof(int16_t);

    std::vector<uint8_t> input_samples;
    std::vector<uint8_t> output_samples;
    input_samples.reserve(input_bytes_to_read);
    output_samples.reserve(max_output_bytes);

    xEventGroupSetBits(this_microphone->event_group_, MicrophoneEventGroupBits::TASK_RUNNING);

    while (!(xEventGroupGetBits(this_microphone->event_group_) & MicrophoneEventGroupBits::COMMAND_STOP)) {
      if (this_microphone->data_callbacks_.size() == 0) {
        vTaskDelay(pdMS_TO_TICKS(READ_DURATION_MS));
        continue;
      }

      input_samples.resize(input_bytes_to_read);
      size_t bytes_read =
          this_microphone->read_(input_samples.data(), input_bytes_to_read, 2 * pdMS_TO_TICKS(READ_DURATION_MS));
      const size_t input_sample_count = bytes_read / sizeof(int32_t);
      if (input_sample_count < channel_count * 3) {
        continue;
      }

      output_samples.resize(max_output_bytes);
      const int32_t *in = reinterpret_cast<const int32_t *>(input_samples.data());
      int16_t *out = reinterpret_cast<int16_t *>(output_samples.data());
      size_t out_samples = 0;

      for (size_t frame = 0; (frame + 2) * channel_count < input_sample_count; frame += 3) {
        const size_t base = frame * channel_count;
        for (uint8_t channel = 0; channel < channel_count; channel++) {
          out[out_samples++] = static_cast<int16_t>(in[base + channel] >> 16);
        }
      }

      output_samples.resize(out_samples * sizeof(int16_t));
      if (this_microphone->correct_dc_offset_) {
        this_microphone->fix_dc_offset_(output_samples);
      }
      this_microphone->data_callbacks_.call(output_samples);
    }
  }

  xEventGroupSetBits(this_microphone->event_group_, MicrophoneEventGroupBits::TASK_STOPPED);
  while (true) {
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void XMOSMicrophone::loop() {
  uint32_t event_group_bits = xEventGroupGetBits(this->event_group_);

  if (event_group_bits & MicrophoneEventGroupBits::TASK_STARTING) {
    ESP_LOGV(TAG, "Task started, attempting to allocate buffer");
    xEventGroupClearBits(this->event_group_, MicrophoneEventGroupBits::TASK_STARTING);
  }

  if (event_group_bits & MicrophoneEventGroupBits::TASK_RUNNING) {
    ESP_LOGV(TAG, "Task is running and reading XMOS data");
    xEventGroupClearBits(this->event_group_, MicrophoneEventGroupBits::TASK_RUNNING);
    this->state_ = microphone::STATE_RUNNING;
  }

  if (event_group_bits & MicrophoneEventGroupBits::TASK_STOPPED) {
    ESP_LOGV(TAG, "Task finished, freeing resources and uninstalling driver");
    vTaskDelete(this->task_handle_);
    this->task_handle_ = nullptr;
    this->stop_driver_();
    xEventGroupClearBits(this->event_group_, ALL_BITS);
    this->status_clear_error();
    this->state_ = microphone::STATE_STOPPED;
  }

  if ((uxSemaphoreGetCount(this->active_listeners_semaphore_) < MAX_LISTENERS) &&
      (this->state_ == microphone::STATE_STOPPED)) {
    this->state_ = microphone::STATE_STARTING;
  }

  if ((uxSemaphoreGetCount(this->active_listeners_semaphore_) == MAX_LISTENERS) &&
      (this->state_ == microphone::STATE_RUNNING)) {
    this->state_ = microphone::STATE_STOPPING;
  }

  switch (this->state_) {
    case microphone::STATE_STARTING:
      if (this->status_has_error()) {
        break;
      }

      if (!this->start_driver_()) {
        ESP_LOGE(TAG, "Driver failed to start; retrying in 1 second");
        this->status_momentary_error("driver_fail", 1000);
        this->stop_driver_();
        break;
      }

      this->configure_voice_stream_settings_();
      if (this->task_handle_ == nullptr) {
        xTaskCreate(XMOSMicrophone::mic_task, "xmos_mic_task", TASK_STACK_SIZE, this, TASK_PRIORITY,
                    &this->task_handle_);

        if (this->task_handle_ == nullptr) {
          ESP_LOGE(TAG, "Task failed to start, retrying in 1 second");
          this->status_momentary_error("task_fail", 1000);
          this->stop_driver_();
        }
      }
      break;

    case microphone::STATE_RUNNING:
      break;
    case microphone::STATE_STOPPING:
      xEventGroupSetBits(this->event_group_, MicrophoneEventGroupBits::COMMAND_STOP);
      break;
    case microphone::STATE_STOPPED:
      break;
  }
}

}  // namespace esphome::i2s_audio

#endif  // USE_ESP32
