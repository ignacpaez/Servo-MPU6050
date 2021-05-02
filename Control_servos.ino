#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#define MPU 0x68  // Dirección I2C del MPU-6050

Servo ServoX, ServoY;
double AcX,AcY,AcZ;
int Pitch, Roll;

void setup(){
  Serial.begin(9600);
  ServoX.attach(2);
  ServoY.attach(3);
  init_MPU(); // iniciar sensor
}
 
void loop()
{
  FunctionsMPU(); // Adquirimos los ejex AcX, AcY, AcZ.
    
  Roll = FunctionsPitchRoll(AcX, AcZ, AcY);   //Calculo del angulo del Roll(x)
  Pitch = FunctionsPitchRoll(AcY, AcX, AcZ);  //Calculo del angulo del Pitch(y)

  int ServoRoll = map(Roll, -90, 90, 0, 179);
  int ServoPitch = map(Pitch, -90, 90, 179, 0);

  ServoX.write(ServoRoll);
  ServoY.write(ServoPitch);


  Serial.print("Eje x: "); Serial.print(Pitch);
  Serial.print("\t");
  Serial.print("Eje y: "); Serial.print(Roll);
  Serial.print("\n");

}

void init_MPU(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  
  Wire.write(0);     // setear a cero
  Wire.endTransmission(true);
  delay(1000);
}

//Funcion para el calculo del angulo Pitch(x) y Roll(y)
double FunctionsPitchRoll(double A, double B, double C){
  double DatoA, DatoB, Value;
  DatoA = A;
  DatoB = (B*B) + (C*C);
  DatoB = sqrt(DatoB);
  
  Value = atan2(DatoA, DatoB);
  Value = Value * 180/3.14;
  
  return (int)Value;
}

//Función para adquirir los ejes X, Y, Z del sensor
void FunctionsMPU(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // registros de los ejes
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,6,true);  // requerimos un total de 6 registros
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
}
