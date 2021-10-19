#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
float startAltitude;
bool startFlag, sepFlag, recFlag, landFlag = false;
float currentAltitude = 0.0;
float maxAltitude = 0.0;

void setup() {
  Serial.begin(9600);
  Serial.println("begin");
  bmp.begin();

  startAltitude = getAltitude();

}

void loop() {
  // сохраняем значение прошлой высоты
  float lastAltitude = currentAltitude;
  currentAltitude = getAltitude() - startAltitude;
  // определяем точку взлета, точку запуска системы спасения, высчитываем максимальную высоту
  if (currentAltitude - startAltitude >= 5.0) startFlag = true;
  if (currentAltitude > maxAltitude) maxAltitude = currentAltitude;
  if ((maxAltitude - currentAltitude >= 50)  && startFlag) recFlag = true;
  
  
  Serial.println("CanSAD;" + String(millis()) + ";" + String(currentAltitude) + ";" + String(startFlag) + ";" + String(sepFlag) + ";" + String(recFlag) + ";" + String(landFlag) + "\n");
  
  // TeamID;TIme;Altitude;A;Start point;Separate point;Recovery point;Landing point \n
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
