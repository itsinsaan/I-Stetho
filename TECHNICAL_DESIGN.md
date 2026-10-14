This document explains how the hardware and software fit together. Include the following.
- System overview: What each component does.
- Hardware architecture: ESP32, microphone, power and connections.
- Software architecture: Firmware, I2S acquisition, network transmission and computer-side receiver.
- Data flow: How sound travels from the microphone to the computer.
- Interfaces and protocols: I2S, Wi-Fi, TCP or whichever network protocol the actual code uses.
- Design limitations: Noise, weak signals, clipping, connectivity and missing clinical validation.
- Future architecture: Database, audio metadata, machine-learning pipeline and clinician-facing review interface.

  stethoscope(chest piece) ---> microphone amplifier ---> ESP32 --->power
