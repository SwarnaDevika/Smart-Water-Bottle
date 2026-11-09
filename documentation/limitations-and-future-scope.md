# Limitations and Future Scope – Smart Water Bottle

## 1. Current Limitations

Although the Smart Water Bottle prototype demonstrates sensor-based water monitoring, certain limitations remain in its current implementation.

### 1.1 TDS Measurement Accuracy
- TDS values are estimated using sensor voltage and a polynomial conversion formula.
- Calibration using standard TDS reference solutions is required to improve measurement accuracy.
- Sensor readings may vary depending on temperature and measurement conditions.

### 1.2 Turbidity Measurement
- The current firmware displays raw ADC values and sensor output voltage.
- Conversion into calibrated Nephelometric Turbidity Units (NTU) has not been implemented.
- Reference-sample calibration is required for reliable turbidity measurements.

### 1.3 Battery Monitoring
- The current OLED interface displays a demonstration battery percentage.
- Actual battery voltage measurement and battery-level calculation are not yet implemented.
- Power optimization and battery-life evaluation require further development.

### 1.4 Hydration Tracking
- The prototype displays a daily water goal of 2 litres.
- Actual water consumption and remaining-water calculations have not been implemented.
- Integration of calibrated weight measurements with hydration tracking is a potential enhancement.

### 1.5 Vibration Reminder
- The firmware includes vibration reminder functionality.
- The vibration output remains disabled until a suitable transistor or MOSFET driver circuit is installed.

### 1.6 Water Quality Detection Scope
- The current prototype focuses on temperature, TDS and turbidity sensor readings.
- It does not detect every possible chemical or biological contaminant.
- Sensor-based readings alone cannot establish that water is safe for drinking.

## 2. Future Scope

The project provides several opportunities for further development.

### 2.1 Advanced Water Quality Monitoring
Additional sensors can be explored to monitor parameters such as:
- pH
- Total hardness
- Chloride
- Sulphate
- Iron
- Fluoride
- Nitrate
- Arsenic

Sensor selection, calibration and validation would be required before integrating these parameters into the prototype.

### 2.2 Improved Hydration Tracking
- Implement actual water consumption calculations using load-cell measurements.
- Calculate remaining water quantity.
- Track daily hydration progress.
- Integrate the vibration motor for physical hydration reminders.

### 2.3 Battery Management
- Implement actual battery voltage monitoring.
- Calculate battery percentage.
- Explore power-saving techniques to improve operating duration.
- Support charging while operating, subject to suitable power-management hardware.

### 2.4 City-Level Digital Twin Concept

A proposed research extension is to develop a city-level Digital Twin concept for exploring regional variations in water quality.

The concept aims to:
- Compare water-quality parameters across different cities, such as Chennai and Bangalore.
- Represent geographical water-quality variations.
- Compare available water-quality data against relevant Bureau of Indian Standards (BIS) drinking-water requirements.
- Explore geographical patterns and regional water-quality concerns.

This is a proposed future research direction and is not currently implemented in the prototype.

### 2.5 Connectivity and Data Visualization
- Explore Wi-Fi or Bluetooth connectivity.
- Develop a platform for storing and visualizing historical sensor readings.
- Enable remote access to water-monitoring information.
- Explore mobile application integration.

## 3. Conclusion

The Smart Water Bottle prototype establishes a foundation for sensor-based water monitoring using embedded systems.

Although certain features require further calibration and implementation, the project provides opportunities to expand water-quality monitoring, improve hydration tracking, introduce better power management and explore geographical water-quality analysis through a proposed Digital Twin approach.
