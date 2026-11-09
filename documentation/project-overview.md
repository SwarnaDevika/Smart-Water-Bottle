# Smart Water Bottle – Project Overview

## 1. Introduction

The Smart Water Bottle is an ESP32-based water monitoring prototype designed to improve drinking water awareness through sensor-based monitoring and embedded systems.

The prototype integrates multiple sensors with an ESP32 microcontroller to collect water-related measurements and display the information through an OLED display.

The system is designed to explore real-time monitoring of water temperature, Total Dissolved Solids (TDS), turbidity and water weight.

## 2. Problem Statement

Access to information about drinking water conditions is important for maintaining awareness of water quality.

Conventional water bottles do not provide electronic monitoring or real-time feedback about water-related parameters.

This project explores the development of a smart monitoring system that integrates sensors and embedded technology to provide users with accessible measurements and reminders.

## 3. Project Objectives

- Develop an ESP32-based smart water monitoring prototype.
- Integrate multiple sensors for monitoring water-related parameters.
- Display sensor readings through an OLED display.
- Implement temperature-based warning messages.
- Explore weight measurement using a load cell and HX711 amplifier.
- Implement hydration reminders through the display.
- Establish a foundation for future water-quality monitoring enhancements.

## 4. Hardware Components

The prototype uses the following components:

- ESP32-WROOM-32 Development Board
- SSD1306 OLED Display
- DS18B20 Waterproof Temperature Sensor
- TDS Sensor
- Turbidity Sensor
- 5 kg Load Cell
- HX711 Load Cell Amplifier
- Vibration Motor (driver-dependent)
- Breadboard and Jumper Wires

## 5. Software and Technologies

- Arduino IDE
- Embedded C/C++
- ESP32 Arduino Framework
- OneWire Library
- DallasTemperature Library
- Adafruit GFX Library
- Adafruit SSD1306 Library
- HX711 Library

## 6. System Functionality

The ESP32 acts as the central controller and processes readings obtained from the connected sensors.

The OLED display provides multiple screens for presenting:

- Water temperature and temperature warnings.
- Weight measurements.
- TDS readings in estimated ppm.
- Turbidity sensor voltage and ADC readings.
- Battery percentage demonstration.
- Daily water consumption goal display.
- Hydration reminders.

The firmware also includes load-cell tare and calibration routines through the Serial Monitor.

## 7. Project Scope

The current project focuses on developing a hardware prototype with sensor interfacing, embedded programming and OLED-based monitoring.

The firmware includes functionality for temperature, TDS, turbidity and weight measurements. The extent of physical integration and testing of each sensor is documented separately in the testing results.

Battery percentage and daily water goal screens currently use demonstration values. The vibration reminder output is disabled until a suitable driver circuit is installed.

## 8. Future Enhancements

The proposed future enhancements include:

- Expanding water-quality monitoring capabilities.
- Exploring additional contaminant detection.
- Comparing regional water-quality parameters against BIS standards.
- Developing a city-level Digital Twin concept to explore geographical variations in water quality, including comparisons between Chennai and Bangalore.
- Improving battery monitoring and power management.
- Integrating additional notification and connectivity features.

The city-level Digital Twin and BIS-based comparison are proposed future research directions and are not part of the currently implemented prototype.

## 9. Conclusion

The Smart Water Bottle project demonstrates the integration of embedded systems, sensor interfacing and real-time display technologies in a practical water monitoring application.

The prototype provides a foundation for further development of smart water monitoring solutions and future research into geographically informed water-quality analysis.
