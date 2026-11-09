# ESP32 Pin Configuration

This document describes the GPIO pin assignments used in the Smart Water Bottle prototype.

## 1. Pin Mapping

| Component | ESP32 GPIO Pin | Function |
--->
| DS18B20 Temperature Sensor | GPIO 4 | Temperature data |
| HX711 DOUT | GPIO 18 | Data output |
| HX711 SCK | GPIO 19 | Clock input |
| TDS Sensor | GPIO 34 | Analog input |
| Turbidity Sensor | GPIO 35 | Analog input |
| Vibration Motor Driver | GPIO 27 | Digital output |
| OLED SDA | GPIO 21* | I2C data |
| OLED SCL | GPIO 22* | I2C clock |

*OLED pins assume the standard ESP32 I2C configuration; confirm against the physical connections.*

## 2. Communication Interfaces

- **OneWire:** Used for communication with the DS18B20 temperature sensor.
- **I2C:** Used for communication with the SSD1306 OLED display.
- **Analog Input:** Used to read voltage outputs from the TDS and turbidity sensors.
- **HX711 Interface:** Used to obtain weight measurements from the load cell.
- **Digital Output:** GPIO 27 is assigned for the vibration reminder driver.

## 3. Important Hardware Notes

- The DS18B20 uses a OneWire data connection.
- The HX711 interfaces the load cell with the ESP32.
- TDS and turbidity sensor outputs are read using ESP32 ADC pins.
- Ensure analog input voltages do not exceed the ESP32's permitted input voltage.
- The vibration motor must use a suitable transistor or MOSFET driver. It must not be connected directly to an ESP32 GPIO pin.
- The vibration driver is currently disabled in the firmware.
- Your TDS and turbidity sensors may use a 5V supply, but ESP32 GPIO inputs are not 5V tolerant. Make sure their analog output voltage is safely within the ESP32 input range before connecting them.

## 4. Firmware Configuration

The pin assignments are defined in `SmartBottle.ino`.

The OLED display is initialized using I2C address `0x3C`.
