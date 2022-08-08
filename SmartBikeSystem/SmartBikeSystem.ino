#include "settings.h"
volatile int interruptCounter;
int totalInterruptCounter;

bool stateLed=false;
bool stateLedIzq=true;
bool stateLedDer=true;
bool stateLedStop=false;
bool stateLedFront=false;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR timerLed() {
  portENTER_CRITICAL_ISR(&timerMux);
  stateLed=!stateLed;
  if(stateLedDer)
  digitalWrite(ledDer,stateLed);
  if(stateLedIzq)
  digitalWrite(ledIzq,stateLed);
  if(stateLedStop)
  digitalWrite(ledStop,stateLed);
  if(stateLedFront)
  digitalWrite(ledFront,stateLed);
  portEXIT_CRITICAL_ISR(&timerMux);
 
}
 
void setup() {
 
  Serial.begin(115200);
  pinMode(ledDer, OUTPUT);
  pinMode(ledIzq, OUTPUT);
  pinMode(ledStop, OUTPUT);
  pinMode(ledFront, OUTPUT);
 
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &timerLed, true);
  timerAlarmWrite(timer, 150000, true);
  timerAlarmEnable(timer);
 
}
 
void loop() {

}