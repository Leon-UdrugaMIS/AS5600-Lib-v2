# AS5600-Lib-v2

Advanced AS5600 / AS5600L library for Arduino and Dasduino CORE with
wraparound-aware calibration, working-zero support, and EEPROM persistence.

## Features
- AS5600 / AS5600L I2C driver (angle, status, configuration registers).
- Wraparound-safe calibration between 0° and 180°.
- Working-zero offset for displays and mechanisms.
- EEPROM persistence for calibration and working-zero data.
- Angular speed and cumulative position helpers.

## Hardware & core compatibility
- Designed for AS5600 and AS5600L sensors.
- Targeted for Dasduino CORE (ATmega328P) and compatible Arduino AVR boards.
- I2C on Dasduino CORE: **SDA=A4**, **SCL=A5**.
- Uses `Wire` and the Arduino `EEPROM` library (no `EEPROM.begin()` needed on AVR).

## Quick start (raw angle)
```cpp
#include <Wire.h>
#include <AS5600.h>

AS5600 sensor;

void setup() {
  Wire.begin();
  sensor.begin();
}

void loop() {
  uint16_t raw = sensor.rawAngle();               // 0..4095
  float degrees = raw * AS5600_RAW_TO_DEGREES;    // 0..360
  (void)degrees;
}
```

## Calibration + working zero
`AS5600Calibrator` handles wraparound and scaling to a 0°–180° range.

```cpp
AS5600Calibrator calibrator;

void setup() {
  calibrator.loadFromEEPROM(); // loads or initializes defaults
}

void saveCalibration(uint16_t rawAt0, uint16_t rawAt180) {
  calibrator.setCalibrationMin(rawAt0);
  calibrator.setCalibrationMax(rawAt180);
  calibrator.setWorkingZeroCentiDegrees(0);
  calibrator.saveToEEPROM();
}

float displayAngle(uint16_t raw) {
  return calibrator.computeDisplayAngleDegrees(raw);
}
```

### Notes
- All calibration math is wraparound-safe (4095 → 0 transition).
- Working-zero is stored in **centi-degrees** (0–18000).
- Use `computeCalibratedAngleDegrees()` when you need the full calibrated range,
  and `computeDisplayAngleDegrees()` for the working-zero display value.

## EEPROM persistence
`AS5600Calibrator::loadFromEEPROM()` and `saveToEEPROM()` store the
`AS5600CalibrationSettings` struct (signature + min/max + working-zero).

- Default EEPROM address is `0`, but you can pass a custom offset:
  `loadFromEEPROM(address)` / `saveToEEPROM(address)`.
- Save **only when values change** (after calibration or working-zero update)
  to limit EEPROM wear.
- `loadFromEEPROM()` returns `true` if valid data is found, otherwise it writes
  defaults and returns `false`.

## Advanced sensor controls
Use these configuration helpers when tuning the sensor:
- `setPowerMode`, `setHysteresis`, `setOutputMode`, `setPWMFrequency`
- `setSlowFilter`, `setFastFilter`, `setWatchDog`
- `getAngularSpeed`, `getCumulativePosition`, `getRevolutions`

## AS5600L address changes
```cpp
AS5600L sensorL;                // defaults to 0x40
sensorL.setAddress(0x41);        // updates runtime I2C address
```
Note: making the new address permanent requires OTP burn operations, which are
not exposed by this library—refer to the AS5600L datasheet.

## Examples
- `examples/AS5600_calibration/AS5600_calibration.ino` demonstrates:
  - 0° / 180° calibration with wraparound handling
  - working-zero adjustment
  - EEPROM persistence
  - Dasduino CORE wiring and button flow
