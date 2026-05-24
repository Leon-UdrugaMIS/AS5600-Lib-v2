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
  float radPerSec = sensor.getAngularSpeed(AS5600_MODE_RADIANS, false);
  float rpm = sensor.getAngularSpeed(AS5600_MODE_RPM, false);
  float rps = sensor.getAngularSpeed(AS5600_MODE_RPS, false);

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
