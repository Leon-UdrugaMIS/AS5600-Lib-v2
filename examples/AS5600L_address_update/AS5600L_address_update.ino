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
