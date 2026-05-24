#include <Wire.h>
#include <AS5600.h>

AS5600 sensor;
AS5600Calibrator calibrator;

const uint32_t SERIAL_BAUD = 115200;
const int EEPROM_ADDRESS = 0;
const unsigned long READ_DELAY_MS = 100;

uint16_t readRawAngle()
{
  return sensor.rawAngle() & AS5600_RAW_MASK;
}

void waitForUser(const char *message)
{
  Serial.println(message);
  while (!Serial.available())
  {
    delay(10);
  }
  while (Serial.available())
  {
    Serial.read();
  }
  delay(100);
}

void runCalibration()
{
  Serial.println("Starting 0-180 calibration.");

  waitForUser("Place the magnet at 0 degrees, then send any key.");
  uint16_t rawAt0 = readRawAngle();
  calibrator.setCalibrationMin(rawAt0);
  Serial.print("Saved 0 deg raw = ");
  Serial.println(rawAt0);

  waitForUser("Place the magnet at 180 degrees, then send any key.");
  uint16_t rawAt180 = readRawAngle();
  calibrator.setCalibrationMax(rawAt180);
  calibrator.setWorkingZeroCentiDegrees(0);
  calibrator.saveToEEPROM(EEPROM_ADDRESS);

  Serial.print("Saved 180 deg raw = ");
  Serial.println(rawAt180);
  Serial.println("Calibration stored in EEPROM.");
}

void setup()
{
  Serial.begin(SERIAL_BAUD);
  Wire.begin();

  if (!sensor.begin())
  {
    Serial.println("AS5600 not detected.");
    while (true)
    {
      delay(1000);
    }
  }

  bool hasCalibration = calibrator.loadFromEEPROM(EEPROM_ADDRESS);
  if (!hasCalibration)
  {
    Serial.println("No calibration in EEPROM.");
    runCalibration();
  }
  else
  {
    Serial.println("Calibration loaded from EEPROM.");
    Serial.println("Send 'c' to recalibrate.");
  }
}

void loop()
{
  if (Serial.available())
  {
    char command = static_cast<char>(Serial.read());
    if (command == 'c' || command == 'C')
    {
      runCalibration();
    }
    while (Serial.available())
    {
      Serial.read();
    }
  }

  uint16_t raw = readRawAngle();
  float protractorDeg = calibrator.computeCalibratedAngleDegrees(raw);

  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.print("  Protractor (0-180): ");
  Serial.println(protractorDeg, 2);
  delay(READ_DELAY_MS);
}

