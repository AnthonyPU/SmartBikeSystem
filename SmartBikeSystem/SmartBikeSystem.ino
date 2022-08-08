#include "settings.h"
#include <Wire.h>
const int mpuAddress = 0x68;          // I2C address of the MPU-6050
bool stateSirena=false;
bool stateLed=false;
bool stateLedDer=false;
bool stateLedIzq=false;
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

void IRAM_ATTR isrDer() {
  if(!FLAG_DER){
    FLAG_DER=true;
    STATE_DER=!STATE_DER;
    if(!STATE_DER){
      digitalWrite(ledDer,false);
      stateLedDer=false;
      Serial.println("Luces derecha desactivadas");
    }
    
  }
}
void IRAM_ATTR isrIzq() {
  if(!FLAG_IZQ){
    FLAG_IZQ=true;
    STATE_IZQ=!STATE_IZQ;
    if(!STATE_IZQ){
      digitalWrite(ledIzq,false);
      stateLedIzq=false;
      Serial.println("Luces izquierda desactivadas");
    }
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
  pinMode(ledDer, OUTPUT);
  pinMode(ledIzq, OUTPUT);
  pinMode(ledStop, OUTPUT);
  pinMode(ledFront, OUTPUT);
  pinMode(PIN_BTN_DER,INPUT_PULLUP);
  pinMode(PIN_BTN_IZQ,INPUT_PULLUP);
  pinMode(PIN_BTN_LOCK,INPUT_PULLUP);
  attachInterrupt(PIN_BTN_DER, isrDer, FALLING);
  attachInterrupt(PIN_BTN_IZQ, isrIzq, FALLING);
  attachInterrupt(PIN_BTN_LOCK, isrLock, FALLING);
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &timerLed, true);
  timerAlarmWrite(timer, 150000, true);
  timerAlarmEnable(timer);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
  Wire.begin();
  // Initialize the MPU-6050 and test if it is connected.
  Wire.beginTransmission( mpuAddress);
  Wire.write( 0x6B);                           // PWR_MGMT_1 register
  Wire.write( 0);                              // set to zero (wakes up the MPU-6050)
  auto error = Wire.endTransmission();
  if( error != 0)
  {
    Serial.println(F( "Error, MPU-6050 not found"));
    for(;;);                                   // halt the sketch if error encountered
  }

}

void loop() {
  if(STATE_DER){
    Serial.println("Luces derecha activadas");
    stateLedDer=true;
    delay(1500);
    FLAG_DER=false;
  }
  if(STATE_IZQ){
    Serial.println("Luces izquierda activadas");
    stateLedIzq=true;
    delay(1500);
    FLAG_IZQ=false;
  }
  if(STATE_LOCK){
    Serial.println("Candado activado");
    antirrobo();
    if(stateSirena)
    alarma();
    delay(1500);
    FLAG_LOCK=false;
  }
}

void antirrobo()
{
  Wire.beginTransmission( mpuAddress);
  Wire.write( 0x3B);                   // Starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission( false);        // No stop condition for a repeated start

  // The MPU-6050 has the values as signed 16-bit integers.
  // There are 7 values in 14 registers.
  int16_t AcX, AcY, AcZ;

  Wire.requestFrom( mpuAddress, 14);   // request a total of 14 bytes
  AcX = Wire.read()<<8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY = Wire.read()<<8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read()<<8 | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
/*
  Serial.print("AcX: ");
  Serial.print(AcX);
  Serial.print("\t");
  Serial.print("AcY: ");
  Serial.print(AcY);
  Serial.print("\t");
  Serial.print("AcZ: ");
  Serial.println(AcZ);
*/
  if(AcX!=0||AcY!=0||AcZ!=0){
    stateSirena=true;
  }

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
