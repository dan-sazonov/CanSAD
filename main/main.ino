#define srv_pin A3  // пин сервы
#define photo_pin A6 // пин фоторезистора
#define srv_high_time 5000  // время работы сервы

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

float start_altitude;
bool start_flag, sep_flag, rec_flag, land_flag = false;
float current_altitude = 0.0;
float max_altitude = 0.0;

unsigned long last_time = 0;
bool srv_state = false;
bool srv_timeout = false;

int photo_val;

void setup() {
  pinMode(srv_pin, OUTPUT);
  pinMode(photo_pin, INPUT);
  Serial.begin(9600);
  Serial.println("begin");
  bmp.begin();

  photo_val = analogRead(photo_pin);
  start_altitude = getAltitude();

}

void loop() {
  // сохраняем значение прошлой высоты
  float last_altitude = current_altitude;
  current_altitude = getAltitude() - start_altitude;
  
  // определяем необходимые точки, высчитываем максимальную высоту
  if (current_altitude - start_altitude >= 5.0) start_flag = true;
  if (current_altitude > max_altitude) max_altitude = current_altitude;
  if ((max_altitude - current_altitude >= 50)  && start_flag) rec_flag = true;
  if (last_altitude - current_altitude <= 2 && start_flag) land_flag = true;
  if (photo_val - analogRead(photo_pin) >= 10) sep_flag = true;
  
  // запускаем серву на 2 секунды
  if (rec_flag && !srv_state) {
    srv_state = true && !srv_timeout;
    last_time = millis();
    digitalWrite(srv_pin, srv_state);
  }
  if (millis() - last_time >= srv_high_time && srv_state) {
    srv_state = false;
    srv_timeout = true;
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
