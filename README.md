# AS5600-Lib-v2

Advanced AS5600 / AS5600L library for Arduino and Dasduino CORE.

This version adds practical calibration support for real mechanisms:
- Wraparound-safe 0° ↔ 180° mapping
- Adjustable working zero (display/reference offset)
- EEPROM persistence for calibration settings

If you need both low-level register access and easy angle display logic, this
library gives you both in one API.

---

## 1) What this library gives you

### Core sensor control (`AS5600`, `AS5600L`)
- Read raw and processed angle values
- Read status, AGC, and magnitude registers
- Configure sensor options (power mode, filters, output mode, watchdog...)
- Estimate angular speed
- Track cumulative position / revolutions

### Calibration utilities (`AS5600Calibrator`)
- Convert raw 12-bit angle (`0..4095`) into calibrated `0.00°..180.00°`
- Handle wraparound automatically (for example, when min is near `4095` and max is near `0`)
- Apply a working zero offset for user-facing displays
- Save and load calibration settings from EEPROM

---

## 2) Hardware and compatibility

- Supports **AS5600** and **AS5600L**
- Designed for **Dasduino CORE (ATmega328P)** and compatible Arduino AVR boards
- Uses Arduino `Wire` and `EEPROM`
- On AVR boards, no `EEPROM.begin()` is required

### Dasduino CORE I2C pins
- **SDA = A4**
- **SCL = A5**

---

## 3) Installation

1. Put this library in your Arduino libraries folder (or install through your preferred library workflow).
2. Include it in your sketch:

```cpp
#include <Wire.h>
#include <AS5600.h>
```

---

## 4) Quick start (read angle in degrees)

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

---

## 5) Calibration model (important)

`AS5600Calibrator` maps a section of the full sensor circle to **0°..180°**:

- `minVal` = raw value you treat as **0°**
- `maxVal` = raw value you treat as **180°**
- Range is computed with circular math, so wraparound is safe

Then an optional **working zero** is subtracted from that calibrated angle:

- Stored in centi-degrees (`0..18000`)
- Useful when your mechanism’s "visual zero" is not the calibration minimum

### Output methods
- `computeCalibratedAngleDegrees(raw)` → full calibrated value in degrees
- `computeDisplayAngleDegrees(raw)` → calibrated value with working-zero applied

---

## 6) Recommended calibration workflow

1. Move mechanism to physical 0° and record raw sensor value.
2. Move mechanism to physical 180° and record raw sensor value.
3. Save both values to calibrator.
4. Set working zero (often `0`, but configurable).
5. Save settings to EEPROM.

```cpp
AS5600Calibrator calibrator;

void setup() {
  calibrator.loadFromEEPROM(); // loads existing data or initializes defaults
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

---

## 7) EEPROM behavior

`AS5600Calibrator` stores `AS5600CalibrationSettings`:
- `signature`
- `minVal`
- `maxVal`
- `workingZeroCentiDegrees`

### Key details
- Default EEPROM address is `0`
- You can use a custom address offset:
  - `loadFromEEPROM(address)`
  - `saveToEEPROM(address)`
- `loadFromEEPROM()` returns:
  - `true` when valid data is found
  - `false` when data is invalid (defaults are restored and written)

### EEPROM wear recommendation
Only call `saveToEEPROM()` when values actually change.

---

## 8) Important API groups

### Angle and status reading
- `rawAngle()`
- `readAngle()`
- `readStatus()`
- `readAGC()`
- `readMagnitude()`
- `magnetDetected()`, `magnetTooStrong()`, `magnetTooWeak()`

### Sensor configuration
- `setPowerMode()` / `getPowerMode()`
- `setHysteresis()` / `getHysteresis()`
- `setOutputMode()` / `getOutputMode()`
- `setPWMFrequency()` / `getPWMFrequency()`
- `setSlowFilter()` / `getSlowFilter()`
- `setFastFilter()` / `getFastFilter()`
- `setWatchDog()` / `getWatchDog()`

### Motion helpers
- `getAngularSpeed(mode, update)`
- `getCumulativePosition(update)`
- `getRevolutions()`
- `resetPosition()`
- `resetCumulativePosition()`

---

## 9) AS5600L address changes

```cpp
AS5600L sensorL;         // default 0x40
sensorL.setAddress(0x41); // runtime address update
```

This updates runtime communication address.  
Making address changes permanent requires OTP burn operations that are **not**
exposed by this library (see AS5600L datasheet).

---

## 10) Common pitfalls and tips

- **No sensor response on I2C:** check power, GND, SDA/SCL wiring, and board pin mapping.
- **Jumping angle values near zero:** use calibration mapping + wraparound-safe logic (already built in).
- **Unexpected display zero:** verify `workingZeroCentiDegrees`.
- **EEPROM values "not loading":** ensure signature/offset location matches your saved settings.
- **Speed estimate unstable:** sample frequently and consistently (avoid long irregular delays).

---

## 11) Example sketch

See:
- `examples/AS5600_calibration/AS5600_calibration.ino`

It demonstrates:
- 0° / 180° calibration
- wraparound-safe mapping
- working-zero adjustment
- EEPROM persistence
- Dasduino CORE button/wiring flow
