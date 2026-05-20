# AS5600-Lib-v2 tutorials and full sketches

Each section below is a complete sketch with all required includes.

## 1) Basic raw angle (I2C read)
```cpp
#include <Wire.h>
#include <AS5600.h>

AS5600 sensor;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  if (!sensor.begin())
  {
    while (true) { delay(1000); }
  }
}

void loop()
{
  uint16_t raw = sensor.rawAngle();
  float degrees = raw * AS5600_RAW_TO_DEGREES;
  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.print("  Degrees: ");
  Serial.println(degrees, 2);
  delay(100);
}
```

## 2) Read filtered angle + software offset
```cpp
#include <Wire.h>
#include <AS5600.h>

AS5600 sensor;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  sensor.begin();
  sensor.setOffset(15.0f);
  sensor.increaseOffset(-5.0f);
}

void loop()
{
  uint16_t angle = sensor.readAngle();
  float degrees = angle * AS5600_RAW_TO_DEGREES;
  float offset = sensor.getOffset();
  Serial.print("Angle: ");
  Serial.print(degrees, 2);
  Serial.print("  Offset: ");
  Serial.println(offset, 2);
  delay(100);
}
```

## 3) Magnet status, AGC, magnitude, and error check
```cpp
#include <Wire.h>
#include <AS5600.h>

AS5600 sensor;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  sensor.begin();
}

void loop()
{
  bool detected = sensor.magnetDetected();
  bool tooStrong = sensor.magnetTooStrong();
  bool tooWeak = sensor.magnetTooWeak();
  uint8_t agc = sensor.readAGC();
  uint16_t magnitude = sensor.readMagnitude();
  int error = sensor.lastError();

  Serial.print("Detected: ");
  Serial.print(detected);
  Serial.print("  TooStrong: ");
  Serial.print(tooStrong);
  Serial.print("  TooWeak: ");
  Serial.print(tooWeak);
  Serial.print("  AGC: ");
  Serial.print(agc);
  Serial.print("  Magnitude: ");
  Serial.print(magnitude);
  Serial.print("  Error: ");
  Serial.println(error);
  delay(250);
}
```

## 4) Direction control (software pin or internal)
```cpp
#include <Wire.h>
#include <AS5600.h>

const uint8_t DIRECTION_PIN = 2;
AS5600 sensor;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  sensor.begin(DIRECTION_PIN);
  sensor.setDirection(AS5600_COUNTERCLOCK_WISE);
}

void loop()
{
  uint8_t direction = sensor.getDirection();
  uint16_t angle = sensor.readAngle();
  Serial.print("Direction: ");
  Serial.print(direction);
  Serial.print("  Angle: ");
  Serial.println(angle);
  delay(100);
}
```

## 5) ZPOS/MPOS/MAX angle registers + full configuration register
```cpp
#include <Wire.h>
#include <AS5600.h>

AS5600 sensor;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  sensor.begin();

  sensor.setZPosition(0);
  sensor.setMPosition(2048);
  sensor.setMaxAngle(4095);

  sensor.setConfiguration(0x0000);
}

void loop()
{
  uint16_t zpos = sensor.getZPosition();
  uint16_t mpos = sensor.getMPosition();
  uint16_t mang = sensor.getMaxAngle();
  uint16_t conf = sensor.getConfiguration();

  Serial.print("ZPOS: ");
  Serial.print(zpos);
  Serial.print("  MPOS: ");
  Serial.print(mpos);
  Serial.print("  MANG: ");
  Serial.print(mang);
  Serial.print("  CONF: 0x");
  Serial.println(conf, HEX);
  delay(250);
}
```

## 6) Configuration helpers (power, hysteresis, output, PWM, filters, watchdog)
```cpp
#include <Wire.h>
#include <AS5600.h>

AS5600 sensor;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  sensor.begin();

  sensor.setPowerMode(AS5600_POWERMODE_NOMINAL);
  sensor.setHysteresis(AS5600_HYST_LSB1);
  sensor.setOutputMode(AS5600_OUTMODE_PWM);
  sensor.setPWMFrequency(AS5600_PWM_460);
  sensor.setSlowFilter(AS5600_SLOW_FILT_4X);
  sensor.setFastFilter(AS5600_FAST_FILT_LSB9);
  sensor.setWatchDog(AS5600_WATCHDOG_ON);
}

void loop()
{
  Serial.print("Power: ");
  Serial.print(sensor.getPowerMode());
  Serial.print("  Hyst: ");
  Serial.print(sensor.getHysteresis());
  Serial.print("  Out: ");
  Serial.print(sensor.getOutputMode());
  Serial.print("  PWM: ");
  Serial.print(sensor.getPWMFrequency());
  Serial.print("  Slow: ");
  Serial.print(sensor.getSlowFilter());
  Serial.print("  Fast: ");
  Serial.print(sensor.getFastFilter());
  Serial.print("  WD: ");
  Serial.println(sensor.getWatchDog());
  delay(500);
}
```

## 7) Angular speed (degrees, radians, RPM, RPS)
```cpp
#include <Wire.h>
#include <AS5600.h>

AS5600 sensor;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  sensor.begin();
}

void loop()
{
  float degPerSec = sensor.getAngularSpeed(AS5600_MODE_DEGREES);
  float radPerSec = sensor.getAngularSpeed(AS5600_MODE_RADIANS);
  float rpm = sensor.getAngularSpeed(AS5600_MODE_RPM);
  float rps = sensor.getAngularSpeed(AS5600_MODE_RPS);

  Serial.print("deg/s: ");
  Serial.print(degPerSec, 2);
  Serial.print("  rad/s: ");
  Serial.print(radPerSec, 2);
  Serial.print("  rpm: ");
  Serial.print(rpm, 2);
  Serial.print("  rps: ");
  Serial.println(rps, 2);
  delay(200);
}
```

## 8) Cumulative position, revolutions, reset helpers
```cpp
#include <Wire.h>
#include <AS5600.h>

AS5600 sensor;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  sensor.begin();
}

void loop()
{
  int32_t position = sensor.getCumulativePosition();
  int32_t revolutions = sensor.getRevolutions();

  Serial.print("Position: ");
  Serial.print(position);
  Serial.print("  Revolutions: ");
  Serial.println(revolutions);

  if (revolutions > 5)
  {
    sensor.resetCumulativePosition(0);
  }

  delay(200);
}
```

## 9) AS5600L address update
```cpp
#include <Wire.h>
#include <AS5600.h>

AS5600L sensorL;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  sensorL.begin();
  sensorL.setAddress(0x41);
}

void loop()
{
  Serial.print("AS5600L address: 0x");
  Serial.println(sensorL.getAddress(), HEX);
  delay(1000);
}
```

## 10) Calibrator API walkthrough (all functions)
```cpp
#include <Wire.h>
#include <AS5600.h>

AS5600 sensor;
AS5600Calibrator calibrator;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  sensor.begin();

  bool hadData = calibrator.loadFromEEPROM(0);
  if (!hadData)
  {
    calibrator.resetToDefaults();
  }

  calibrator.setCalibrationMin(120);
  calibrator.setCalibrationMax(2100);
  calibrator.setCalibration(120, 2100);

  uint16_t minVal = calibrator.getCalibrationMin();
  uint16_t maxVal = calibrator.getCalibrationMax();
  uint16_t diff = calibrator.circularDifference(minVal, maxVal);
  uint16_t range = calibrator.calibratedRange();

  calibrator.setWorkingZeroCentiDegrees(500);
  uint16_t workingZero = calibrator.getWorkingZeroCentiDegrees();

  AS5600CalibrationSettings snapshot = calibrator.settings();
  snapshot.workingZeroCentiDegrees = workingZero;
  calibrator.setSettings(snapshot);

  calibrator.saveToEEPROM(0);

  Serial.print("Min: ");
  Serial.print(minVal);
  Serial.print("  Max: ");
  Serial.print(maxVal);
  Serial.print("  Diff: ");
  Serial.print(diff);
  Serial.print("  Range: ");
  Serial.print(range);
  Serial.print("  Zero: ");
  Serial.println(workingZero);
}

void loop()
{
  uint16_t raw = sensor.rawAngle();
  uint16_t calibratedCenti = calibrator.computeCalibratedAngleCentiDegrees(raw);
  uint16_t displayCenti = calibrator.computeDisplayAngleCentiDegrees(raw);
  float calibratedDeg = calibrator.computeCalibratedAngleDegrees(raw);
  float displayDeg = calibrator.computeDisplayAngleDegrees(raw);

  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.print("  Calibrated (cd): ");
  Serial.print(calibratedCenti);
  Serial.print("  Display (cd): ");
  Serial.print(displayCenti);
  Serial.print("  Cal (deg): ");
  Serial.print(calibratedDeg, 2);
  Serial.print("  Disp (deg): ");
  Serial.println(displayDeg, 2);
  delay(150);
}
```
