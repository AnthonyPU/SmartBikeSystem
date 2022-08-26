#include "settings.h"
#include <Wire.h>

//Declaración de interrupciones de los botones
void IRAM_ATTR isrDer();
void IRAM_ATTR isrIzq();
void IRAM_ATTR isrBlz();
void IRAM_ATTR isrLock();
//Declaración de la interrupción del encoder
void IRAM_ATTR isrEncoder();
//Declaración de la interrupción del timer para parpadeo
void IRAM_ATTR timerLed();

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando Programa");
  // Inicialización de los pines
  pinMode(LED_DER, OUTPUT);
  pinMode(LED_IZQ, OUTPUT);
  pinMode(LED_STOP, OUTPUT);
  pinMode(LED_FRONT, OUTPUT);
  pinMode(PIN_ENCODER, INPUT_PULLUP);
  pinMode(PIN_BTN_DER, INPUT_PULLUP);
  pinMode(PIN_BTN_IZQ, INPUT_PULLUP);
  pinMode(PIN_BTN_LOCK, INPUT_PULLUP);
  // Inicialización de las interrupciones
  attachInterrupt(PIN_BTN_DER, isrDer, FALLING);
  attachInterrupt(PIN_BTN_IZQ, isrIzq, FALLING);
  attachInterrupt(PIN_BTN_BLZ, isrBlz, FALLING);
  attachInterrupt(PIN_BTN_LOCK, isrLock, FALLING);
  attachInterrupt(PIN_ENCODER, isrEncoder, FALLING);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
  // Inicialización del timer
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &timerLed, true);
  timerAlarmWrite(timer, TIEMPO_PARPADEO, true);
  timerAlarmEnable(timer);
  // Inicialización del MPU6050
  Wire.begin();
  Wire.beginTransmission( mpuAddress);
  Wire.write( 0x6B);
  Wire.write( 0);
  auto error = Wire.endTransmission();
  if ( error != 0)
  {
    Serial.println(F( "Error, MPU-6050 not found"));
    for (;;);
  }
  //Encendido del LED_FRONT
  digitalWrite(LED_FRONT, HIGH);
}

void loop() {
  //Condicionales de acuerdo a la activación de cada interrupción
  //El delay permite el efecto rebote en los pulsadores
  if (flag_encoder) {
    delay(TIEMPO_REBOTE);
    flag_encoder = false;
  }
  if (flag_der) {
    Serial.println("Luces derecha");
    delay(TIEMPO_REBOTE);
    flag_der = false;
  }
  if (flag_izq) {
    Serial.println("Luces izquierda");
    delay(TIEMPO_REBOTE);
    flag_izq = false;
  }
  if (flag_blz) {
    Serial.println("Luces baliza");
    delay(TIEMPO_REBOTE);
    flag_blz = false;
  }
  if (state_lock) {
    Serial.println("Candado activado");
    digitalWrite(LED_FRONT, LOW);
    delay(TIEMPO_REBOTE);
    flag_lock = false;
    guardarDatosIniciales();
    while (!state_sirena){
      antirrobo();
    }
    digitalWrite(LED_FRONT, HIGH);
    alarma();
  }
  else{
    //Mostrar los valores de velocidad y distancia
    Serial.print("Velocidad: ");
    Serial.print(vel);
    Serial.print(" km/h");
    Serial.print("\t");
    Serial.print("Distancia recorrida: ");
    Serial.print(dist);
    Serial.println(" km");
  }
  //Condicional para apagar las luces cuando la bicicleta no recorre
  if (millis() - t_lastPulse > 5000) {
    digitalWrite(LED_STOP, LOW);
    state_led_stop = false;
  }
}

//Funciones de interrupción

void IRAM_ATTR isrDer() {
  if (!flag_der) {
    state_led_der = !state_led_der;
    flag_der = true;
  }
}
void IRAM_ATTR isrIzq() {
  if (!flag_izq) {
    state_led_izq = !state_led_izq;
    flag_izq = true;
  }
}
void IRAM_ATTR isrBlz() {
  if (!flag_blz) {
    state_led_izq = !state_led_izq;
    state_led_der = !state_led_der;
    flag_blz = true;
  }
}
void IRAM_ATTR isrLock() {
  if (!flag_lock) {
    flag_lock = true;
    state_lock = !state_lock;
    if (!state_lock) {
      state_sirena = false;
      state_led_der = false;
      state_led_izq = false;
      state_led_stop = false;
      state_led_front = false;
      digitalWrite(LED_DER, LOW);
      digitalWrite(LED_IZQ, LOW);
      digitalWrite(LED_STOP, LOW);
      digitalWrite(LED_FRONT, LOW);
      Serial.println("Candado desactivado");
    }
  }
}
void IRAM_ATTR isrEncoder() {
  if (!flag_encoder) {
    flag_encoder = true;
    dt = millis() - t_lastPulse;
    t_lastPulse = millis(),
    dist = diametro * PI * 0.0000254 + dist;
    vel = diametro * PI * 0.0000254 / dt * 3600000.0;
    digitalWrite(LED_FRONT, true);
    if (vel_last < vel) {
      digitalWrite(LED_STOP, true);
    }
    else {
      digitalWrite(LED_STOP, false);
    }
    vel_last = vel;
  }
}
void IRAM_ATTR timerLed() {
  portENTER_CRITICAL_ISR(&timerMux);
  state_led = !state_led;
  if (state_led_der)
    digitalWrite(LED_DER, state_led);
  else
    digitalWrite(LED_DER, false);
  if (state_led_izq)
    digitalWrite(LED_IZQ, state_led);
  else
    digitalWrite(LED_IZQ, false);
  if (state_led_stop)
    digitalWrite(LED_STOP, state_led);
  /*
  if (state_led_front)
    digitalWrite(LED_FRONT, state_led);
  */
  portEXIT_CRITICAL_ISR(&timerMux);
}
//Función para guardar datos iniciales
void guardarDatosIniciales()
{
  Wire.beginTransmission( mpuAddress);
  Wire.write( 0x3B);                   // Starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission( false);        // No stop condition for a repeated start
  // The MPU-6050 has the values as signed 16-bit integers.
  // There are 7 values in 14 registers.

  Wire.requestFrom( mpuAddress, 14);   // request a total of 14 bytes
  AcX0 = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY0 = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ0 = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp0=Wire.read()<<8|Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX0=Wire.read()<<8|Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY0=Wire.read()<<8|Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ0=Wire.read()<<8|Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  Serial.print("AcX0: ");
  Serial.print(AcX0);
  Serial.print("\t");
  Serial.print("AcY0: ");
  Serial.print(AcY0);
  Serial.print("\t");
  Serial.print("AcZ0: ");
  Serial.print(AcZ0);
  Serial.print("\t");
  Serial.print("GyX0: ");
  Serial.print(GyX0);
  Serial.print("\t");
  Serial.print("GyY0: ");
  Serial.print(GyY0);
  Serial.print("\t");
  Serial.print("GyZ0: ");
  Serial.println(GyZ0);
  
}
//Función para detectar movimiento
void antirrobo()
{
  Wire.beginTransmission( mpuAddress);
  Wire.write( 0x3B);                   // Starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission( false);        // No stop condition for a repeated start
  // The MPU-6050 has the values as signed 16-bit integers.
  // There are 7 values in 14 registers.
  int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

  Wire.requestFrom( mpuAddress, 14);   // request a total of 14 bytes
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  Serial.print("AcX: ");
  Serial.print(AcX);
  Serial.print("\t");
  Serial.print("AcY: ");
  Serial.print(AcY);
  Serial.print("\t");
  Serial.print("AcZ: ");
  Serial.print(AcZ);
  Serial.print("\t");
  Serial.print("GyX: ");
  Serial.print(GyX);
  Serial.print("\t");
  Serial.print("GyY: ");
  Serial.print(GyY);
  Serial.print("\t");
  Serial.print("GyZ: ");
  Serial.println(GyZ);
  
  if (abs(GyX-GyX0) > MARGEN_GYR || abs(GyY-GyY0) > MARGEN_GYR || abs(GyZ-GyZ) > MARGEN_GYR) {
    state_sirena = true;
    state_led_der = true;
    state_led_izq = true;
    state_led_stop = true;
    state_led_front = true;
  }
}

//Funciones para generar melodías en el buzzer

void zap1() {
  for (float f = 3000; f > 40; f = f * 0.93) {
    ledcWriteTone(BUZZER_CHANNEL, f);
    delay(10);
  }
}
void zap2() {
  for (float f = 3000; f > 10; f = f * 0.85) {
    ledcWriteTone(BUZZER_CHANNEL, 2 * f);
    delay(5);
    ledcWriteTone(BUZZER_CHANNEL, f);
    delay(5);
  }
}
void risefall() {
  float rise_fall_time = 180;
  int steps = 50;
  float f_max = 2600;
  float f_min = 1000;
  float delay_time = rise_fall_time / steps;
  float step_size = (f_max - f_min) / steps;
  for (float f = f_min; f < f_max; f += step_size) {
    ledcWriteTone(BUZZER_CHANNEL, f);
    delay(delay_time);
  }
  for (float f = f_max; f > f_min; f -= step_size) {
    ledcWriteTone(BUZZER_CHANNEL, f);
    delay(delay_time);
  }
}
void fall(float rise_fall_time) {
  int steps = 50;
  float f_max = 2000;
  float f_min = 500;
  float delay_time = rise_fall_time / steps;
  float step_size = 0.97;
  for (float f = f_max; f > f_min; f *= step_size) {
    ledcWriteTone(BUZZER_CHANNEL, f);
    delay(delay_time);
  }
}
void rise() {
  float rise_fall_time = 2000;
  int steps = 100;
  float f_max = 1500;
  float f_min = 500;
  float delay_time = rise_fall_time / steps;
  float step_size = 1.012;
  for (float f = f_min; f < f_max; f *= step_size) {
    ledcWriteTone(BUZZER_CHANNEL, f);
    delay(delay_time);
  }
  ledcWriteTone(BUZZER_CHANNEL, 0);
  delay(100);
}
void twotone() {
  float f_max = 1500;
  float f_min = 1000;
  float delay_time = 800;
  ledcWriteTone(BUZZER_CHANNEL, f_max);
  delay(delay_time);
  ledcWriteTone(BUZZER_CHANNEL, f_min);
  delay(delay_time);
}
void alarma() {
  for (int count = 1; count <= 10; count++) {
    if (!state_lock)
      break;
    risefall();
  }
  ledcWriteTone(BUZZER_CHANNEL, 0);
  delay(GAP);
  for (int count = 1; count <= 10; count++) {
    if (!state_lock)
      break;
    fall(300);
  }
  ledcWriteTone(BUZZER_CHANNEL, 0);
  delay(GAP);
  for (int count = 1; count <= 5; count++) {
    if (!state_lock)
      break;
    fall(600);
  }
  ledcWriteTone(BUZZER_CHANNEL, 0);
  delay(GAP);
  for (int count = 1; count < 5; count++) {
    if (!state_lock)
      break;
    rise();
  }
  ledcWriteTone(BUZZER_CHANNEL, 0);
  delay(GAP);
  for (int count = 1; count < 5; count++) {
    if (!state_lock)
      break;
    twotone();
  }
  ledcWriteTone(BUZZER_CHANNEL, 0);
  delay(GAP);
  for (int count = 1; count < 10; count++) {
    if (!state_lock)
      break;
    zap1();
  }
  ledcWriteTone(BUZZER_CHANNEL, 0);
  delay(GAP);
  for (int count = 1; count < 10; count++) {
    if (!state_lock)
      break;
    zap2();
  }
  ledcWriteTone(BUZZER_CHANNEL, 0);
  delay(GAP);
  flag_lock = false;
}