#include "settings.h"

void IRAM_ATTR isrDer() {
  if(!FLAG_DER){
    FLAG_DER=true;
    STATE_DER=!STATE_DER;
    if(!STATE_DER)
    Serial.println("Derecha Desactivado");
  }
}
void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando Programa");
  pinMode(PIN_BTN_DER,INPUT_PULLUP);
  attachInterrupt(PIN_BTN_DER, isrDer, FALLING);
}

void loop() {
  if(STATE_DER){
    Serial.println("Derecha activado");
    delay(1000);
    FLAG_DER=false;
  }
}

