# Atmosfera Echo Hub

The Atmosfera Echo Hub is the ultimate Home Assistant companion, seamlessly integrating voice control, climate management, and smart device connectivity.

Designed to create a harmonious living environment, this multifunctional hub combines advanced presence and climate sensors with a powerful voice-controlled satellite speaker, allowing you to effortlessly manage your home.

With built-in ZigBee and Matter support, the Atmosfera Echo Hub connects a wide range of smart devices to Home Assistant, ensuring a cohesive ecosystem.

Its intuitive voice assistant responds to your commands, enabling you to adjust settings, monitor conditions, and control devices with ease.

Experience the future of home automation with the Atmosfera Echo Hub, where comfort, connectivity, and control converge.

# Setup:

1. Install VSCode
2. Install Python
3. Install GIT
4. In VSCode
   1. Clone this repo
   2. Install dependences:
      ```bash
      pip install esphome pillow==10.2.0 libmagic python-magic-bin==0.4.14 pip-system-certs
      ```
5. Compile with `esphome -v compile .\atmosfera-echo-hub.yaml`
6. Press Ctrl+Shift+P and select `Wokwi: Start Simulator`

![](https://esphome.io/_images/made-for-esphome-white-on-black.svg)
