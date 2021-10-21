# define srv_pin A3

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

float start_altitude;
bool start_flag, sep_flag, rec_flag, land_flag = false;
float current_altitude = 0.0;
float max_altitude = 0.0;

unsigned long last_time;
bool srv_is_working = false;

void setup() {
  pinMode(srv_pin, OUTPUT);
  Serial.begin(9600);
  Serial.println("begin");
  bmp.begin();

  start_altitude = getAltitude();

}

void loop() {
  // сохраняем значение прошлой высоты
  float last_altitude = current_altitude;
  current_altitude = getAltitude() - start_altitude;
  
  // определяем точку взлета, точку запуска системы спасения, высчитываем максимальную высоту
  if (current_altitude - start_altitude >= 5.0) start_flag = true;
  if (current_altitude > max_altitude) max_altitude = current_altitude;
  if ((max_altitude - current_altitude >= 50)  && start_flag) rec_flag = true;

  // запускаем серву на 2 секунды
  if (rec_flag) {
    digitalWrite(srv_pin, HIGH);
  }

  // отправляем телеметрию
  Serial.println("CanSAD;" + String(millis()) + ";" + String(current_altitude) + ";" + String(start_flag) + ";" + String(sep_flag) + 
  ";" + String(rec_flag) + ";" + String(land_flag) + "\n");
  
}


float getAltitude() {
  sensors_event_t event;
  bmp.getEvent(&event);
    if (event.pressure) {
    float sea_level_pressure = SENSORS_PRESSURE_SEALEVELHPA;
    return bmp.pressureToAltitude(sea_level_pressure,event.pressure);
  }
  else return 0.0;
}
