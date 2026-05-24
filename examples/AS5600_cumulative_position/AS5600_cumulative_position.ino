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
