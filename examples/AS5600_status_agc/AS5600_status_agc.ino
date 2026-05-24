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

