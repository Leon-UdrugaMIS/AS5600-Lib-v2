#include <Wire.h>
#include <AS5600.h>

AS5600 sensor;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  if (!sensor.begin())
  {
    Serial.println("AS5600 not detected.");
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

