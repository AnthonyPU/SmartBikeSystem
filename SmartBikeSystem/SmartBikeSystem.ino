#include "settings.h"
#include <Wire.h>
const int mpuAddress = 0x68;          // I2C address of the MPU-6050
bool stateSirena=false;

void setup()
{
  Serial.begin( 115200);
  Serial.println("Iniciando programa");
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

void loop(){
  antirrobo();
  if(stateSirena)
  sirena();
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
void sirena(){
  Serial.println("iuuuu iuuuu iuuuuuu iuuuu iuuuu iuuuuuu");
}
