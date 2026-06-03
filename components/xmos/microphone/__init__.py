from esphome import pins
import esphome.codegen as cg
from esphome.components import audio, esp32, microphone
import esphome.config_validation as cv
from esphome.const import (
    CONF_BITS_PER_SAMPLE,
    CONF_CHANNEL,
    CONF_ID,
    CONF_NUM_CHANNELS,
    CONF_SAMPLE_RATE,
)
from esphome.components.i2s_audio import (
    CONF_I2S_DIN_PIN,
    CONF_LEFT,
    CONF_MONO,
    CONF_PDM,
    CONF_RIGHT,
    CONF_STEREO,
    I2SAudioIn,
    i2s_audio_component_schema,
    i2s_audio_ns,
    register_i2s_audio_component,
    validate_mclk_divisible_by_3,
)

CODEOWNERS = ["@kyvaith"]
DEPENDENCIES = ["i2s_audio"]

CONF_ADC_TYPE = "adc_type"
CONF_CORRECT_DC_OFFSET = "correct_dc_offset"
CONF_ENABLED = "enabled"


XMOSMicrophone = i2s_audio_ns.class_(
    "XMOSMicrophone", I2SAudioIn, microphone.Microphone, cg.Component
)


def _validate_esp32_variant(config):
    variant = esp32.get_esp32_variant()
    if config[CONF_ADC_TYPE] == "external":
        if config[CONF_PDM]:
            raise cv.Invalid("PDM is not supported for the XMOS microphone integration.")
        return config
    raise cv.Invalid(f"{variant} supports XMOS microphones only as external I2S input.")


def _validate_channel(config):
    if config[CONF_CHANNEL] == CONF_MONO:
        raise cv.Invalid(f"I2S microphone does not support {CONF_MONO}.")
    return config


def _set_num_channels_from_config(config):
    config[CONF_NUM_CHANNELS] = 1 if config[CONF_CHANNEL] in (CONF_LEFT, CONF_RIGHT) else 2
    return config


def _set_voice_stream_limits(config):
    # The XMOS stream is captured as 48 kHz / 32-bit I2S, then converted in C++
    # to 16 kHz / 16-bit PCM for ESPHome Voice Assistant and microWakeWord.
    audio.set_stream_limits(
        min_bits_per_sample=16,
        max_bits_per_sample=16,
        min_channels=config.get(CONF_NUM_CHANNELS),
        max_channels=config.get(CONF_NUM_CHANNELS),
        min_sample_rate=16000,
        max_sample_rate=16000,
    )(config)
    return config


def _supported_xmos_settings(config):
    if config[CONF_BITS_PER_SAMPLE] != 32:
        raise cv.Invalid("XMOS I2S capture must be configured as 32bit.")
    if config[CONF_SAMPLE_RATE] != 48000:
        raise cv.Invalid("XMOS I2S capture must be configured as 48kHz.")
    return config


BASE_SCHEMA = microphone.MICROPHONE_SCHEMA.extend(
    i2s_audio_component_schema(
        XMOSMicrophone,
        default_sample_rate=48000,
        default_channel=CONF_STEREO,
        default_bits_per_sample="32bit",
    ).extend(
        {
            cv.Optional(CONF_CORRECT_DC_OFFSET, default=False): cv.boolean,
            cv.Optional(CONF_ENABLED, default=True): cv.boolean,
        }
    )
).extend(cv.COMPONENT_SCHEMA)


CONFIG_SCHEMA = cv.All(
    cv.typed_schema(
        {
            "external": BASE_SCHEMA.extend(
                {
                    cv.Required(CONF_I2S_DIN_PIN): pins.internal_gpio_input_pin_number,
                    cv.Optional(CONF_PDM, default=False): cv.boolean,
                }
            ),
        },
        key=CONF_ADC_TYPE,
    ),
    _supported_xmos_settings,
    _validate_esp32_variant,
    _validate_channel,
    _set_num_channels_from_config,
    _set_voice_stream_limits,
    validate_mclk_divisible_by_3,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await register_i2s_audio_component(var, config)
    await microphone.register_microphone(var, config)

    cg.add(var.set_din_pin(config[CONF_I2S_DIN_PIN]))
    cg.add(var.set_pdm(config[CONF_PDM]))
    cg.add(var.set_correct_dc_offset(config[CONF_CORRECT_DC_OFFSET]))
    cg.add(var.set_enabled(config[CONF_ENABLED]))
