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
