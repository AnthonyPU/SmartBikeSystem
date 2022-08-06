#include "settings.h"

void IRAM_ATTR isrDer() {
  if(!FLAG_DER){
    FLAG_DER=true;
    STATE_DER=!STATE_DER;
    if(!STATE_DER)
    Serial.println("Luces derecha desactivadas");
  }
}
void IRAM_ATTR isrIzq() {
  if(!FLAG_IZQ){
    FLAG_IZQ=true;
    STATE_IZQ=!STATE_IZQ;
    if(!STATE_IZQ)
    Serial.println("Luces izquierda desactivadas");
  }
}
void IRAM_ATTR isrLock() {
  if(!FLAG_LOCK){
    FLAG_LOCK=true;
    STATE_LOCK=!STATE_LOCK;
    if(!STATE_LOCK)
    Serial.println("Candado desactivado");
  }
}
void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando Programa");
  pinMode(PIN_BTN_DER,INPUT_PULLUP);
  pinMode(PIN_BTN_IZQ,INPUT_PULLUP);
  pinMode(PIN_BTN_LOCK,INPUT_PULLUP);
  attachInterrupt(PIN_BTN_DER, isrDer, FALLING);
  attachInterrupt(PIN_BTN_IZQ, isrDer, FALLING);
  attachInterrupt(PIN_BTN_LOCK, isrDer, FALLING);
}

void loop() {
  if(STATE_DER){
    Serial.println("Luces derecha activadas");
    delay(1000);
    FLAG_DER=false;
  }
  if(STATE_IZQ){
    Serial.println("Luces izquierda activadas");
    delay(1000);
    FLAG_IZQ=false;
  }
  if(STATE_LOCK){
    Serial.println("Candado activado");
    delay(1000);
    FLAG_LOCK=false;
  }
}

