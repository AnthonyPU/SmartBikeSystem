#include "settings.h"
long t_lastPulse;
float dt;
float diametro = 26;
float dist=0,vel=0;
bool FLAG_ENCODER=false;
void IRAM_ATTR isrEncoder() {
  if(!FLAG_ENCODER){
    FLAG_ENCODER=true;
    dt=millis()-t_lastPulse;
    t_lastPulse=millis(),
    dist=diametro*PI*0.0000254+dist;
    vel=diametro*PI*0.0000254/dt*3600000.0;
  }
}
void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando Programa");
  pinMode(PIN_ENCODER,INPUT_PULLUP);
  attachInterrupt(PIN_ENCODER,isrEncoder, FALLING);
}

void loop() {
  if(FLAG_ENCODER){
    Serial.print("Velocidad: ");
    Serial.print(vel);
    Serial.print(" km/h");
    Serial.print("\t");
    Serial.print("Distancia recorrida: ");
    Serial.print(dist);
    Serial.println(" km");
    delay(200);
    FLAG_ENCODER=false;
  }
}