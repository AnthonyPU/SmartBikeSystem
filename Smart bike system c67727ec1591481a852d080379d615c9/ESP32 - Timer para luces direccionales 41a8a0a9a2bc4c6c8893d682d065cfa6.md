# ESP32 - Timer para luces direccionales

[ESP32 Arduino: interrupciones Timer](https://descubrearduino.com/esp32-arduino-interrupciones-timer/)

```arduino
volatile int interruptCounter;
int totalInterruptCounter;

int led=2;
bool ledState=false;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  ledState=!ledState;
  digitalWrite(led,ledState);
  portEXIT_CRITICAL_ISR(&timerMux);
 
}
 
void setup() {
 
  Serial.begin(115200);
  pinMode(led, OUTPUT);
 
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 100000, true);
  timerAlarmEnable(timer);
 
}
 
void loop() {
 /*
  if (interruptCounter > 0) {
 
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
 
    totalInterruptCounter++;
 
    Serial.print("An interrupt as occurred. Total number: ");
    Serial.print(totalInterruptCounter);
    Serial.print("\t");
    Serial.println(ledState);
 
  }*/
}
```