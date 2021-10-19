#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
int startAltitude;

void setup() {
  Serial.begin(9600);
  Serial.println("begin");
  bmp.begin();

  startAltitude = getAltitude();

}

void loop() {
  Serial.println(getAltitude() - startAltitude);
}


float getAltitude() {
  sensors_event_t event;
  bmp.getEvent(&event);
    if (event.pressure) {
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    return bmp.pressureToAltitude(seaLevelPressure,event.pressure);
  }
  else
  {
    Serial.println("Sensor error");
    return 0.0;
  }
}
