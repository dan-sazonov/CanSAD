// переменная хранения времени (unsigned long)
uint32_t myTimer1;
int period = 1000;
bool status_s = true;
bool was_activated = false;

void setup() {
  Serial.begin(9600);
  }

void loop() {
  if (millis() - myTimer1 >= 500 && !was_activated) {   // ищем разницу (500 мс)
    was_activated = true;
    status_s = false;
  }
  Serial.println(status_s);
}
