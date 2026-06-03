# Atmosfera Echo Hub TODO

- Investigate a lower-power XMOS idle mode for Satellite1-style voice handling: keep wake word quality, but reduce or suspend the heavier AEC/NS/AGC pipeline until an active voice session or playback reference requires it.
- Revisit the manual direct framebuffer snapshot compositor for tile swipes after the broader LVGL performance work. The target baseline is the version after the excessive touch-handler work was removed, but before the manual direct compositor was disabled: swipe UX was good there, and that exact combination is worth restoring once LVGL/render throughput is improved.
- Calibrate display backlight limits: brightness below roughly 70% currently turns the screen fully off. Measure the real usable PWM range for this panel/backlight circuit, then set safe min/max thresholds and remap the UI brightness slider so low values remain visible.
- Evaluate an ESP32-P4 bridge for LVGL's DMA2D draw backend. The upstream LVGL DMA2D backend is currently STM32-register based, so enabling it on this hardware needs a real ESP-IDF/PPA/DMA2D HAL adapter rather than only unfiltering `src/draw/dma2d`.
