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

