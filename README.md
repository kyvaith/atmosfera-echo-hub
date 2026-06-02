# Atmosfera Echo Hub

[![Made for Home Assistant](https://img.shields.io/badge/Made%20for-Home%20Assistant-41BDF5?style=for-the-badge&logo=homeassistant&logoColor=white)](https://www.home-assistant.io/)
[![Powered by ESPHome](https://img.shields.io/badge/Powered%20by-ESPHome-000000?style=for-the-badge&logo=esphome&logoColor=white)](https://esphome.io/)
[![ESP32-P4](https://img.shields.io/badge/ESP32--P4-Espressif-E7352C?style=for-the-badge&logo=espressif&logoColor=white)](https://www.espressif.com/)
[![Zigbee](https://img.shields.io/badge/Zigbee-Ready-EB0443?style=for-the-badge&logo=zigbee&logoColor=white)](https://csa-iot.org/all-solutions/zigbee/)
[![Matter](https://img.shields.io/badge/Matter-Ready-4B5AB8?style=for-the-badge&logo=matter&logoColor=white)](https://csa-iot.org/all-solutions/matter/)
[![Thread](https://img.shields.io/badge/Thread-Ready-00A651?style=for-the-badge&logo=thread&logoColor=white)](https://www.threadgroup.org/)
[![SendSpin](https://img.shields.io/badge/Audio-SendSpin-7C4DFF?style=for-the-badge)](#music-and-audio)
[![Music Assistant](https://img.shields.io/badge/Music-Music%20Assistant-FF6F61?style=for-the-badge)](https://www.music-assistant.io/)
[![LVGL](https://img.shields.io/badge/UI-LVGL-00AEEF?style=for-the-badge&logo=lvgl&logoColor=white)](https://lvgl.io/)

**Atmosfera Echo Hub** is a state-of-the-art Home Assistant satellite: a premium, extremely hackable, ESPHome-native smart home console built to bring voice assistance, audio, ambience, presence, sensors, local control, and AI-aware perception into one beautiful device.

It is designed as the ultimate room companion for Home Assistant: a compact hub with a local wake word voice assistant, visual feedback, music playback, room controls, local sensors, Zigbee/Matter/Thread networking, and an expressive physical interface for automations.

## Vision

Atmosfera Echo Hub is not just a touchscreen dashboard. It is a deeply integrated home endpoint:

- A Home Assistant satellite for every important room.
- A local wake word voice assistant with rich on-screen, RGB, and haptic feedback.
- A 100% ESPHome-based device, built from the ground up to be inspectable, modifiable, and community-hackable.
- A premium music endpoint with rich, physical sound.
- A visual and haptic smart home interface inspired by Material 3 Expressive.
- A local environmental station, presence sensor, camera node, and automation surface.
- A bridge between Home Assistant, Music Assistant, SendSpin multiroom audio, Zigbee, Matter, Thread, and local room intelligence.

## Hardware Highlights

- **Home Assistant satellite**  
  Built to live as a dedicated Home Assistant room endpoint, not as a generic tablet taped to a wall.

- **Voice assistant with local wake word**  
  Designed for Home Assistant Assist, local wake word detection, voice interaction, and expressive visual feedback through the OLED display, RGB ring, and haptic motor.

- **100% ESPHome foundation**  
  The firmware is ESPHome-first, making the device transparent, reproducible, OTA-friendly, and exceptionally hackable.

- **3-inch True Color 800x800px round OLED display**  
  A compact, high-contrast, always-available visual interface for media, sensors, notifications, controls, scenes, and camera previews.

- **Deep natural audio system**  
  Atmosfera Echo Hub uses a surface exciter to turn the surface beneath it into a subwoofer-like resonating body, paired with a renowned Dayton Audio PC83-4 3-inch driver for clean mids and highs.

- **Zigbee, Matter, and Thread hub**  
  Built-in MultiPAN support enables Zigbee and Matter/Thread workflows, with integration paths for ZHA or Zigbee2MQTT.

- **360-degree AI-aware camera coverage**  
  Four built-in cameras actively observe the surroundings, enabling panoramic perception and AI interpretation of the room context.

- **Four presence sensors**  
  Multi-zone presence awareness gives the hub a richer understanding of room occupancy and interaction intent.

- **Environmental sensing array**  
  Designed for room-grade telemetry: temperature, humidity, pressure, CO2, and laser PM2.5 sensing, with room for additional air-quality and comfort metrics.

- **IR blaster**  
  Integrated infrared control for air conditioners, TVs, receivers, and other legacy appliances.

- **RGB feedback ring**  
  A built-in RGB ring provides glanceable status, ambient cues, notifications, timers, voice states, and automation feedback.

- **Haptic motor**  
  Physical feedback makes touch navigation feel deliberate, responsive, and natural.

## Software Experience

Atmosfera Echo Hub is designed to feel like a purpose-built smart home appliance, while remaining fully open to customization.

### Voice Assistant

- Home Assistant Assist satellite experience.
- Local wake word detection for fast, private, room-native activation.
- Visual listening, thinking, speaking, and error states on the OLED display.
- RGB ring feedback for wake word, command capture, processing, and response.
- Haptic confirmation for successful activation, rejected commands, and critical alerts.
- Future-ready voice flows for music, climate, lights, scenes, reminders, timers, and room status.

### Music And Audio

- Music player integration with Music Assistant.
- Multiroom playback powered by SendSpin.
- Local playback controls: play, pause, previous, next, volume, progress, metadata, and now-playing artwork or animation.
- Room-aware audio endpoint behavior for whole-home listening.
- Future-ready support for grouped playback, announcements, doorbell chimes, alarms, reminders, and spoken Home Assistant feedback.

### Home Assistant Control Surface

The interface can expose configurable tiles and pages for Home Assistant entities and actions, including:

- Switches
- Lights
- Actions
- Triggers
- Sensors
- Sliders
- Covers
- Garage doors
- Locks
- Date and time
- World clocks
- Weather
- Media players
- Climate controls
- Internal switches
- Subpages

If Home Assistant can see it, Atmosfera Echo Hub is designed to make it feel native on a dedicated room device.

### Sensors And Room Awareness

Atmosfera Echo Hub can read and display local sensors directly while also publishing them back into Home Assistant:

- Temperature
- Humidity
- Barometric pressure
- CO2
- PM2.5 particulate matter
- Presence and occupancy
- Light level
- Noise level
- Air-quality trend
- Room comfort score
- Device health and connectivity state

The goal is not only to show raw values, but to transform them into practical room intelligence: comfort, ventilation quality, occupancy, and automation readiness.

### Cameras And Context

With four cameras covering 360 degrees, the hub is designed for rich local context:

- Live camera preview.
- Configurable automatic camera popups.
- Doorbell-triggered camera preview.
- Room event awareness.
- AI-assisted interpretation of what is happening around the device.
- Privacy-first automation modes, including local-only processing targets where possible.

### Notifications

Atmosfera Echo Hub can become the room's notification endpoint:

- Home Assistant notifications.
- Doorbell alerts.
- Security and alarm states.
- Washer, dryer, dishwasher, and appliance completion alerts.
- Calendar and reminder prompts.
- Air-quality warnings.
- Presence-aware notification routing.
- RGB ring and haptic notification feedback.

### Interface Design

The UI follows the spirit of **Material 3 Expressive**:

- Bold, glanceable surfaces.
- Intentional motion and feedback.
- High-contrast OLED-friendly composition.
- Clear hierarchy for media, sensors, rooms, and alerts.
- Touch-first controls designed for quick physical interaction.
- Consistent visual language across tiles, notifications, media, and settings.

## Onboarding Experience

Atmosfera Echo Hub is intended to deliver a frictionless onboarding experience:

1. Flash the ESPHome firmware.
2. Connect the device to WiFi.
3. Adopt it in Home Assistant.
4. Configure tiles, rooms, sensors, audio, and automations.
5. Update over the air as the project evolves.

The long-term goal is a polished first-run flow where the device feels like a real product, not a loose collection of components.

## What Else This Device Could Support

Atmosfera Echo Hub has enough hardware surface area to grow into a truly ultimate Home Assistant satellite. High-value expansion areas include the following.

### Additional Sensors

- VOC sensor for paints, cleaning products, cooking fumes, and indoor air quality.
- NOx sensor for deeper air-quality interpretation.
- Formaldehyde sensor for furniture, renovation, and material off-gassing.
- Lux and color-temperature sensor for adaptive lighting.
- UV sensor for rooms with strong daylight exposure.
- mmWave radar for fine-grained presence, stillness, and sleep-adjacent sensing.
- Time-of-flight gesture sensor for touchless interactions.
- Thermal array for privacy-preserving occupancy and heat maps.
- SPL/noise sensor for room loudness, alarms, and acoustic comfort.
- Vibration sensor for appliance detection or tamper awareness.
- Smoke, gas, or CO sensor integration as external safety inputs.

### Software Capabilities

- Local text-to-speech and announcement routing.
- Room-level AI summarization: "what changed here?", "is the room comfortable?", "why is the air quality bad?".
- Context-aware automations using presence, cameras, audio state, and environmental sensors together.
- AI camera snapshots for doorbell, package, pet, person, and room-state recognition.
- Adaptive dashboards that change by time of day, presence, media state, or active automation.
- Scene launcher with haptic and RGB confirmation.
- Alarm keypad and security status surface.
- Intercom or push-to-talk room communication.
- Baby monitor or elder-care room awareness modes.
- Energy dashboard for nearby plugs, appliances, and room consumption.
- Climate advisor that combines temperature, humidity, CO2, PM2.5, HVAC state, and window/door sensors.
- Air purifier and ventilation automation assistant.
- Appliance assistant for laundry, dishwasher, oven, and coffee machine notifications.
- Local automation fallback for essential controls when Home Assistant is temporarily unavailable.
- Configurable screensavers: clock, weather, now playing, room comfort, family status, or camera carousel.
- Per-room profiles for bedrooms, kitchen, office, living room, workshop, garage, and hallway.
- Backup and restore for UI layouts and device configuration.
- OTA update channels for stable, beta, and experimental builds.

## Current Firmware Focus

This repository currently focuses on the ESPHome firmware foundation for Atmosfera Echo Hub:

- ESP32-P4 target.
- ESP-IDF framework.
- LVGL-based interface.
- 32-bit LVGL color pipeline using RGB888 output.
- PPA-accelerated rendering.
- MIPI DSI display output.
- Touch input.
- SendSpin media source and multiroom group control.
- I2S audio output through an external DAC.
- Music Assistant-oriented now-playing UI.
- OTA and USB flashing workflow.

## Development

The firmware is built with ESPHome:

```powershell
.\venv\Scripts\esphome.exe compile atmosfera-echo-hub.yaml
```

Upload over USB:

```powershell
.\venv\Scripts\esphome.exe upload atmosfera-echo-hub.yaml --device COM5
```

Upload over OTA:

```powershell
.\venv\Scripts\esphome.exe upload atmosfera-echo-hub.yaml --device atmosfera-echo-hub.local
```

The project is intentionally ESPHome-native. The best parts of it should remain easy to inspect, fork, flash, extend, and repair.

## Project Status

Atmosfera Echo Hub is an ambitious, product-grade smart home satellite concept in active development. The goal is to combine the openness of ESPHome with the polish of a premium consumer device: powerful enough for enthusiasts, approachable enough for daily household use, and hackable enough to become whatever your home needs next.
