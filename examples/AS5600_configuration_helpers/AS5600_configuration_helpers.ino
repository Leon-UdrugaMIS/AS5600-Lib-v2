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

