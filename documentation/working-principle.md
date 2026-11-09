# Working Principle – Smart Water Bottle

## 1. System Overview

The Smart Water Bottle prototype uses an ESP32 microcontroller as its central processing unit. The ESP32 receives readings from connected sensors, processes the measurements and presents the information through an SSD1306 OLED display.

## 2. ESP32 Microcontroller

The ESP32 coordinates the sensor-reading operations and display updates.

Its responsibilities include:
- Reading sensor values.
- Processing analog and digital sensor data.
- Managing OLED screen navigation.
- Generating temperature warnings.
- Managing hydration reminder intervals.
- Handling load-cell calibration commands through the Serial Monitor.

## 3. Temperature Monitoring – DS18B20

The DS18B20 waterproof temperature sensor measures water temperature.

Working process:
1. The sensor communicates with ESP32 through the OneWire protocol.
2. The DallasTemperature library requests temperature readings.
3. The ESP32 retrieves the measured temperature in Celsius.
4. The temperature is displayed on the OLED screen.
5. Temperature-based warning messages are generated.

The firmware uses the following warning thresholds:
- 50°C or above: HOT warning.
- 60°C or above: TOO HOT warning.

## 4. TDS Monitoring

The Total Dissolved Solids (TDS) sensor provides an analog voltage corresponding to the sensor's response to dissolved substances in water.

Working process:
1. The ESP32 reads the analog voltage through GPIO 34.
2. The firmware applies temperature compensation when a valid temperature reading is available.
3. A polynomial calculation converts the compensated voltage into an estimated TDS value in ppm.
4. The calculated value is displayed on the OLED screen.

The calculation depends on sensor calibration and measurement conditions.

## 5. Turbidity Monitoring

The turbidity sensor provides an analog output related to the optical characteristics of water.

Working process:
1. The sensor output is connected to ESP32 GPIO 35.
2. The ESP32 reads the analog-to-digital converter (ADC) value.
3. The corresponding sensor voltage is obtained.
4. The ADC reading and voltage are displayed on the OLED screen.

The current firmware displays raw ADC and voltage readings rather than calibrated turbidity values in NTU.

## 6. Weight Measurement – Load Cell and HX711

The load cell is used to measure weight through the HX711 amplifier module.

Working process:
1. The load cell produces a small electrical signal corresponding to the applied weight.
2. The HX711 amplifies and converts this signal into digital data.
3. The ESP32 receives the data through GPIO 18 (DOUT) and GPIO 19 (SCK).
4. The load cell is tared with an empty platform.
5. A known reference weight is used for calibration.
6. Once calibration is successful, the firmware calculates and displays weight in grams.

Calibration is performed through Serial Monitor commands.

- Send `t` to tare the empty platform.
- Place the known reference weight.
- Send `c` to calibrate.

## 7. OLED Display – SSD1306

The SSD1306 OLED display provides a user interface for presenting sensor readings and system information.

The firmware uses I2C communication to interface with the ESP32.

The display rotates through six screens:
1. Water temperature.
2. Weight measurement.
3. Battery percentage demonstration.
4. TDS reading.
5. Turbidity sensor reading.
6. Daily water goal.

Each screen is updated at a defined time interval.

## 8. Hydration Reminder

The firmware includes an hourly hydration reminder mechanism.

Working process:
1. The ESP32 tracks elapsed time using the `millis()` function.
2. After the configured one-hour interval, a reminder becomes active.
3. The OLED displays a hydration reminder message.
4. The reminder remains visible for approximately eight seconds.

A vibration output is also programmed, but it remains disabled until a suitable motor driver circuit is installed.

## 9. Overall System Workflow

The overall operation follows this sequence:

1. Initialize the ESP32 and connected hardware.
2. Initialize the OLED display and sensor libraries.
3. Read temperature, TDS and turbidity sensor values.
4. Read weight if load-cell calibration is complete.
5. Process the sensor measurements.
6. Update the OLED display.
7. Check the hydration reminder interval.
8. Repeat the process continuously.

## 10. Conclusion

The working principle demonstrates how embedded programming, sensor interfacing and real-time display technologies can be integrated into a smart water monitoring prototype.

The system provides a foundation for further development of water-quality monitoring and hydration-related features.
