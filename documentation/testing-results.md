# Testing Results – Smart Water Bottle

## 1. Testing Overview

Testing was performed to evaluate the functionality of the Smart Water Bottle prototype, including sensor interfacing, temperature monitoring, weight measurement, OLED display operation and firmware behaviour.

The testing process focused on observing sensor readings and verifying communication between the ESP32 and connected components.

## 2. Component Testing

| Component | Test Performed | Observation | Status |
|---|---|---|---|
| ESP32 | Firmware upload and execution | Firmware uploaded and executed successfully | Passed |
| OLED Display | Display initialization and screen navigation | Display operated and presented system information | Passed |
| DS18B20 | Normal and hot water temperature testing | Temperature readings obtained; approximately 28.5°C observed during normal water testing | Tested |
| TDS Sensor | Analog voltage measurement | Sensor voltage readings observed | Tested |
| Turbidity Sensor | ADC and voltage measurement | Analog readings obtained | Tested |
| Load Cell + HX711 | Weight measurement and calibration | Weight measurement functionality completed | Completed |
| Vibration Motor | Reminder output | Driver installation and motor testing pending | Pending |

## 3. Temperature Sensor Testing

The DS18B20 waterproof temperature sensor was tested using water at different temperatures.

**Observations:**
- The sensor successfully provided temperature readings.
- A reading of approximately 28.5°C was observed during normal water testing.
- Normal and hot water were used during testing.
- Temperature readings were displayed on the OLED screen.

## 4. TDS Sensor Testing

The TDS sensor was interfaced with the ESP32 through an analog input pin.

**Observations:**
- Analog voltage readings were obtained.
- The firmware processes the voltage to calculate an estimated TDS value in ppm.
- Temperature compensation is applied when a valid temperature reading is available.
- Further calibration using reference solutions is required to validate measurement accuracy.

## 5. Turbidity Sensor Testing

The turbidity sensor was interfaced with the ESP32 to observe analog output variations.

**Observations:**
- Raw ADC readings were obtained.
- Sensor output voltage was measured.
- The firmware displays ADC and voltage readings on the OLED screen.
- Further calibration against known turbidity reference samples is required to obtain reliable NTU measurements.

## 6. Load Cell and HX711 Testing

The load cell was integrated with the HX711 amplifier module to measure weight.

**Testing activities:**
- Load-cell interfacing with the HX711 module.
- Empty-platform tare operation.
- Reference-weight calibration.
- Weight measurement using the ESP32.
- Display of measured weight in grams.

**Observation:**

The load-cell measurement and calibration functionality was completed as part of the prototype development.

## 7. OLED Display Testing

The SSD1306 OLED display was tested with the ESP32.

**Observations:**
- The display initialized successfully.
- Multiple information screens were displayed.
- Temperature, weight, TDS, turbidity and demonstration information were included in the display navigation.
- Hydration reminder messages were implemented.

## 8. Hydration Reminder Testing

The firmware includes an hourly hydration reminder mechanism.

**Observations:**
- The ESP32 tracks elapsed time using the `millis()` function.
- A reminder is activated after the configured one-hour interval.
- The OLED displays a hydration reminder message.
- The reminder remains visible for approximately eight seconds.

The vibration output remains disabled until a suitable motor driver circuit is installed.

## 9. Incomplete Testing and Limitations

The following features require further testing or implementation:

- Battery voltage monitoring and actual battery percentage calculation.
- Vibration motor operation with a suitable driver circuit.
- TDS calibration using reference solutions.
- Turbidity calibration using reference samples.
- Water consumption and remaining-water calculations.

Additional limitations:

- TDS readings have not been validated against a calibrated reference instrument.
- Turbidity readings are currently displayed as ADC values and voltage rather than calibrated NTU.
- Battery percentage is currently a demonstration value.
- Complete hydration tracking based on actual water consumption has not yet been implemented.

## 10. Conclusion

The testing process demonstrated successful ESP32 firmware execution, OLED display operation, temperature sensor interfacing and load-cell measurement functionality.

Analog readings were also obtained from the TDS and turbidity sensors.

The prototype establishes a foundation for further development of smart water monitoring solutions, with additional calibration, power management and hardware enhancements identified for future work.
