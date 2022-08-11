# ESP32 - Buzzer

[sketch.ino - Wokwi Arduino and ESP32 Simulator](https://wokwi.com/projects/328908980078248532)

[LED Control (LEDC) - Arduino-ESP32 2.0.2 documentation](https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/ledc.html)

[L5: Playing tones](https://makeabilitylab.github.io/physcomp/esp32/tone.html)

![Untitled](ESP32%20-%20Buzzer%20f184e5d21ba74e65b0f9adccf7e0575f/Untitled.png)

[HQ Sound Effect | Car alarm](https://www.youtube.com/watch?v=nuPHwijMsIg)

```arduino
int beep_pin=10;
int gap=1000;

void setup() {
  // put your setup code here, to run once:
  pinMode(beep_pin, OUTPUT);
}

void zap1()
{
    for (float f=3000;f>40;f=f*0.93){
    tone(beep_pin,f);
    delay(10);
  }
}

void zap2()
{
    for (float f=3000;f>10;f=f*0.85){
    tone(beep_pin,2*f);
    delay(5);
    tone(beep_pin,f);
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
    tone(beep_pin,f);
    delay(delay_time);
  }
   for (float f =f_max;f>f_min;f-=step_size){
    tone(beep_pin,f);
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
    tone(beep_pin,f);
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
    tone(beep_pin,f);
    delay(delay_time);
  }
  noTone(beep_pin);
  delay(100);
  
}

void twotone()
{
  float f_max=1500;
  float f_min=1000;
  float delay_time=800;
  tone(beep_pin,f_max);
  delay(delay_time);
  tone(beep_pin,f_min);
  delay(delay_time);
  
}
void loop() {
  // put your main code here, to run repeatedly:

 
  for (int count=1;count<=10;count++)
  {
    risefall();
  }
  noTone(beep_pin);
  delay(gap);
  for (int count=1;count<=10;count++)
  {
    fall(300);
  } 
  noTone(beep_pin);
  delay(gap); 
  for (int count=1;count<=5;count++)
  {
    fall(600);
  }
  noTone(beep_pin);
  delay(gap); 
  for (int count=1;count<5;count++)
  {
    rise();
  }
  noTone(beep_pin);
  delay(gap); 
  for (int count=1;count<5;count++)
  {
    twotone();
  }
  noTone(beep_pin);
  delay(gap); 
  for (int count=1;count<10;count++)
  {
    zap1();
  }
  noTone(beep_pin);
  delay(gap); 
  for (int count=1;count<10;count++)
  {
    zap2();
  }
  noTone(beep_pin);
  delay(gap);  
}
```