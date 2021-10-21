// переменная хранения времени (unsigned long)
uint32_t myTimer1;
void setup() {Serial.begin(9600);}
void loop() {
  if (millis() - myTimer1 >= 500) {   // ищем разницу (500 мс)
    myTimer1 = millis();              // сброс таймера
    Serial.println("test");
  }
}
