#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "HX711.h"

// ===================== OLED DISPLAY =====================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
bool oledReady = false;

// ===================== TEMPERATURE SENSOR =====================
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float waterTemperature = -999.0;

// ===================== LOAD CELL / HX711 =====================
#define HX711_DOUT 18
#define HX711_SCK 19
HX711 scale;
bool loadCellCalibrated = false;
float calibrationFactor = 1.0;
float measuredWeight = -1.0;
// Use an actual reference weight matching this value during calibration.
const float referenceWeightGrams = 14.0;

// ===================== TDS SENSOR =====================
#define TDS_PIN 34
float tdsVoltage = 0.0;
float tdsValue = 0.0;

// ===================== TURBIDITY SENSOR =====================
#define TURBIDITY_PIN 35
float turbidityVoltage = 0.0;
int turbidityRaw = 0;

// ===================== VIBRATION / OLED REMINDER =====================
#define VIBRATOR_PIN 27
// Keep false unless a suitable transistor/MOSFET driver is installed.
// Do not drive a vibration motor directly from an ESP32 GPIO.
const bool vibratorDriverInstalled = false;
const unsigned long reminderIntervalMs = 60UL * 60UL * 1000UL;
const unsigned long reminderDisplayMs = 8000UL;
const unsigned long vibratorPulseMs = 350UL;
unsigned long lastReminderMs = 0;
unsigned long reminderShownAt = 0;
unsigned long vibratorPulseStartedAt = 0;
bool reminderActive = false;
bool reminderPulseActive = false;

// ===================== DISPLAY NAVIGATION =====================
int currentScreen = 0;
unsigned long lastScreenUpdate = 0;
const unsigned long screenInterval = 2500UL;
const int totalScreens = 6;

// ===================== TEMPERATURE READING =====================
float readTemperature() {
  sensors.requestTemperatures();
  if (sensors.getDeviceCount() == 0) return -999.0;
  float tempC = sensors.getTempCByIndex(0);
  if (tempC == DEVICE_DISCONNECTED_C || tempC <= -127.0 || tempC == 85.0 || isnan(tempC)) return -999.0;
  return tempC;
}

// ===================== LOAD CELL TARE =====================
// Remove any reference weight first. Tare with the empty platform/bottle holder.
void calibrateLoadCell() {
  Serial.println("\n===== LOAD CELL TARE =====");
  if (!scale.is_ready()) {
    Serial.println("HX711 not ready. Check the HX711 connection and power.");
    loadCellCalibrated = false;
    measuredWeight = -1.0;
    return;
  }

  scale.set_scale(1.0);
  scale.tare(10);
  loadCellCalibrated = false;
  Serial.println("Tare complete.");
  Serial.println("Now place the known 100 g reference weight on the platform.");
  Serial.println("Send c in Serial Monitor to calibrate.");
}

// ===================== LOAD CELL CALIBRATION =====================
bool setLoadCellCalibration(float knownWeightGrams) {
  if (!scale.is_ready() || knownWeightGrams <= 0.0) {
    Serial.println("Calibration failed: HX711 not ready or reference weight invalid.");
    loadCellCalibrated = false;
    measuredWeight = -1.0;
    return false;
  }

  Serial.println("Reading reference weight...");
  long rawReading = scale.get_value(15);
  if (rawReading == 0) {
    Serial.println("Calibration failed: no measurable change from tare.");
    loadCellCalibrated = false;
    return false;
  }

  calibrationFactor = (float)rawReading / knownWeightGrams;
  scale.set_scale(calibrationFactor);
  float testWeight = scale.get_units(10);

  if (!isfinite(testWeight) || testWeight < knownWeightGrams * 0.5 || testWeight > knownWeightGrams * 1.5) {
    Serial.println("Calibration check failed.");
    Serial.print("Measured test weight: ");
    Serial.print(testWeight, 1);
    Serial.println(" g");
    Serial.println("Make sure the reference weight matches the value in code and remains on the platform.");
    loadCellCalibrated = false;
    scale.set_scale(1.0);
    return false;
  }

  loadCellCalibrated = true;
  Serial.print("Calibration successful. Factor: ");
  Serial.println(calibrationFactor, 4);
  Serial.print("Reference weight reading: ");
  Serial.print(testWeight, 1);
  Serial.println(" g");
  Serial.println("Live weight measurement is enabled.");
  return true;
}

// ===================== LIVE WEIGHT READING =====================

float readWeightGrams() {
  if (!loadCellCalibrated) return -1.0;

  if (!scale.is_ready()) return measuredWeight;

  float weight = scale.get_units(10);

  if (!isfinite(weight)) return measuredWeight;

  if (weight >= -0.5 && weight <= 0.5) {
    weight = 0.0;
  }

  return weight;
}


// ===================== SERIAL CALIBRATION COMMANDS =====================
// Serial Monitor: send t with empty platform; then put known 100 g weight and send c.
void handleCalibrationCommand() {
  while (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 't' || command == 'T') {
      calibrateLoadCell();
    } else if (command == 'c' || command == 'C') {
      setLoadCellCalibration(referenceWeightGrams);
    }
  }
}

// ===================== TDS READING =====================
void readTDS() {
  tdsVoltage = analogReadMilliVolts(TDS_PIN) / 1000.0;
  float compensationCoefficient = 1.0;
  if (waterTemperature != -999.0) {
    compensationCoefficient = 1.0 + 0.02 * (waterTemperature - 25.0);
  }
  float compensationVoltage = tdsVoltage;
  if (waterTemperature != -999.0 && compensationCoefficient > 0.0) {
    compensationVoltage = tdsVoltage / compensationCoefficient;
  }
  tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage
              - 255.86 * compensationVoltage * compensationVoltage
              + 857.39 * compensationVoltage) * 0.5;
  if (tdsValue < 0.0) tdsValue = 0.0;
}

// ===================== TURBIDITY READING =====================
void readTurbidity() {
  turbidityRaw = analogRead(TURBIDITY_PIN);
  turbidityVoltage = analogReadMilliVolts(TURBIDITY_PIN) / 1000.0;
}

// ===================== HYDRATION REMINDER =====================
void updateHydrationReminder() {
  unsigned long now = millis();
  if (!reminderActive && now - lastReminderMs >= reminderIntervalMs) {
    reminderActive = true;
    reminderShownAt = now;
    lastReminderMs = now;
    if (vibratorDriverInstalled) {
      digitalWrite(VIBRATOR_PIN, HIGH);
      reminderPulseActive = true;
      vibratorPulseStartedAt = now;
    }
  }
  if (reminderPulseActive && now - vibratorPulseStartedAt >= vibratorPulseMs) {
    digitalWrite(VIBRATOR_PIN, LOW);
    reminderPulseActive = false;
  }
  if (reminderActive && now - reminderShownAt >= reminderDisplayMs) reminderActive = false;
}

// ===================== OLED SCREENS =====================
void drawCurrentScreen() {
  if (!oledReady) return;
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);

  if (reminderActive) {
    display.println("HYDRATION REMINDER");
    display.setTextSize(2);
    display.setCursor(0, 22);
    display.println("Drink water!");
    display.setTextSize(1);
    display.setCursor(0, 50);
    display.println(vibratorDriverInstalled ? "Vibration sent" : "OLED reminder only");
  } else if (currentScreen == 0) {
    display.println("WATER TEMPERATURE");
    if (waterTemperature == -999.0) {
      display.setTextSize(2);
      display.setCursor(0, 23);
      display.println("Sensor error");
    } else {
      display.setTextSize(2);
      display.setCursor(0, 18);
      display.print(waterTemperature, 1);
      display.println(" C");
      display.setTextSize(1);
      display.setCursor(0, 46);
      if (waterTemperature >= 60.0) display.println("WARNING: TOO HOT!");
      else if (waterTemperature >= 50.0) display.println("WARNING: HOT!");
      else display.println("Check before drinking");
    }
  } else if (currentScreen == 1) {
    display.println("WEIGHT");
    display.setTextSize(3);
    display.setCursor(0, 25);
    if (measuredWeight < 0.0) {
      display.setTextSize(2);
      display.println("Calibrate");
      display.setTextSize(1);
      display.setCursor(0, 51);
      display.println("Send t then c in Serial");
    } else {
      display.print(measuredWeight, 1);
      display.setTextSize(2);
      display.println("g");
    }
  } else if (currentScreen == 2) {
    display.println("BATTERY");
    display.setTextSize(3);
    display.setCursor(25, 24);
    display.println("82%");
  } else if (currentScreen == 3) {
    display.println("TDS READING");
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print(tdsValue, 0);
    display.println(" ppm");
    display.setTextSize(1);
    display.setCursor(0, 43);
    display.print("V: ");
    display.print(tdsVoltage, 2);
    display.println(" V");
    display.setCursor(0, 55);
  } else if (currentScreen == 4) {
    display.println("TURBIDITY SENSOR");
    display.setCursor(0, 20);
    display.print("ADC: ");
    display.println(turbidityRaw);
    display.setCursor(0, 35);
    display.print("Voltage: ");
    display.print(turbidityVoltage, 2);
    display.println(" V");
    display.setCursor(0, 51);
  } else if (currentScreen == 5) {
    display.println("DAILY WATER GOAL");
    display.setTextSize(2);
    display.setCursor(12, 25);
    display.println("2 Litres");
  }
  display.display();
}

// ===================== SETUP =====================
void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(VIBRATOR_PIN, OUTPUT);
  digitalWrite(VIBRATOR_PIN, LOW);
  lastReminderMs = millis();

  Serial.println();
  Serial.println("==============================");
  Serial.println("      SMART WATER BOTTLE      ");
  Serial.println("==============================");
  Serial.println("Serial Monitor baud rate: 115200");
  Serial.println("Load-cell setup: send t with platform empty, then add the known 14 g weight and send c.");

  analogReadResolution(12);
  analogSetPinAttenuation(TDS_PIN, ADC_11db);
  analogSetPinAttenuation(TURBIDITY_PIN, ADC_11db);
  sensors.begin();

  oledReady = display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  if (oledReady) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Smart Water Bottle");
    display.println("Starting...");
    display.display();
  } else {
    Serial.println("OLED not detected.");
  }

  scale.begin(HX711_DOUT, HX711_SCK);
  scale.set_scale(1.0);
  if (scale.is_ready()) {
    Serial.println("HX711 detected.");
    Serial.println("Calibration required before live weight can be shown.");
  } else {
    Serial.println("HX711 not ready.");
  }

  waterTemperature = readTemperature();
  Serial.println("Firmware initialized.");
  Serial.println("OLED screens: temperature, live weight (g), battery demo 82%, TDS, turbidity, daily goal 2 Litres.");
  Serial.println("No water-consumed screen/message is used.");
  Serial.println("Vibrator remains disabled until a suitable transistor/MOSFET driver is installed.");
}

// ===================== MAIN LOOP =====================

void loop() {
  handleCalibrationCommand();

  waterTemperature = readTemperature();
  readTDS();
  readTurbidity();

  measuredWeight = readWeightGrams();

  updateHydrationReminder();

  unsigned long now = millis();

  if (oledReady && now - lastScreenUpdate >= screenInterval) {
    lastScreenUpdate = now;
    drawCurrentScreen();
    currentScreen++;

    if (currentScreen >= totalScreens) {
      currentScreen = 0;
    }
  }

  delay(20);
}

