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
