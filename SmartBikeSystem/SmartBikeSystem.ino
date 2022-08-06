#include "settings.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando Programa");
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
}

void loop() {
  alarma();
}

void melody(){
  for(int i=0;i<CANT_NOTES;i++){
    ledcWriteNote(BUZZER_CHANNEL,NOTAS_MUSIC[i],OCTAVA_NOTA[i]);
    delay(TIEMPO_NOTA[i]);
  }
}
void zap1()
{
    for (float f=3000;f>40;f=f*0.93){
    ledcWriteTone(BUZZER_CHANNEL, f);
    delay(10);
  }
}        

void zap2()
{
    for (float f=3000;f>10;f=f*0.85){
        ledcWriteTone(BUZZER_CHANNEL,2*f);
        delay(5);
        ledcWriteTone(BUZZER_CHANNEL,f);
        delay(5);
  }
}
void risefall()
{
  float rise_fall_time=180;
  int steps=50;
  float f_max=2600;
  float f_min=1000;
  float delay_time=rise_fall_time/steps;
  float step_size=(f_max-f_min)/steps;
  for (float f =f_min;f<f_max;f+=step_size){
    ledcWriteTone(BUZZER_CHANNEL,f);
    delay(delay_time);
  }
   for (float f =f_max;f>f_min;f-=step_size){
    ledcWriteTone(BUZZER_CHANNEL,f);
    delay(delay_time);
  }
}
void fall(float rise_fall_time)
{
  int steps=50;
  float f_max=2000;
  float f_min=500;
  float delay_time=rise_fall_time/steps;
  float step_size=0.97;
  for (float f =f_max;f>f_min;f*=step_size){
    ledcWriteTone(BUZZER_CHANNEL,f);
    delay(delay_time);
  }
}
void rise()
{
  float rise_fall_time=2000;
  int steps=100;
  float f_max=1500;
  float f_min=500;
  float delay_time=rise_fall_time/steps;
  float step_size=1.012;
  for (float f =f_min;f<f_max;f*=step_size){
    ledcWriteTone(BUZZER_CHANNEL,f);
    delay(delay_time);
  }
  ledcWriteTone(BUZZER_CHANNEL,0);
  delay(100);
  
}

void twotone()
{
  float f_max=1500;
  float f_min=1000;
  float delay_time=800;
  ledcWriteTone(BUZZER_CHANNEL,f_max);
  delay(delay_time);
  ledcWriteTone(BUZZER_CHANNEL,f_min);
  delay(delay_time);
  
}
void alarma() {

  for (int count=1;count<=10;count++)
  {
    risefall();
  }
  ledcWriteTone(BUZZER_CHANNEL,0);
  delay(gap);
  for (int count=1;count<=10;count++)
  {
    fall(300);
  } 
  ledcWriteTone(BUZZER_CHANNEL,0);
  delay(gap); 
  for (int count=1;count<=5;count++)
  {
    fall(600);
  }
  ledcWriteTone(BUZZER_CHANNEL,0);
  delay(gap); 
  for (int count=1;count<5;count++)
  {
    rise();
  }
  ledcWriteTone(BUZZER_CHANNEL,0);
  delay(gap); 
  for (int count=1;count<5;count++)
  {
    twotone();
  }
  ledcWriteTone(BUZZER_CHANNEL,0);
  delay(gap); 
  for (int count=1;count<10;count++)
  {
    zap1();
  }
  ledcWriteTone(BUZZER_CHANNEL,0);
  delay(gap); 
  for (int count=1;count<10;count++)
  {
    zap2();
  }
  ledcWriteTone(BUZZER_CHANNEL,0);
  delay(gap);  
}