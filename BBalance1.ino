/*
 * Balance robot using MPU6050
 * Connects to wifi network for control
 * by tablet application for android
 * 
 * this is free ware code use at your leasure
 */
 
#include <Arduino.h>
#include "MPU6050.h"
#include "A4988.h"
#include "ESP8266.h"
#include <Servo.h>

void doInit(void);

#define LEFTPIN 7
#define RIGHTPIN 12
#define LEFTDIR 8
#define RIGHTDIR 5
#define ENABLED 4


MPU6050 imu;
A4988 Mot;
ESP8266 Esp;
Servo S;

// used to set network connection
#define WiFi "your network name"
#define Auth "your network password"

int16_t ax, ay, az;
int16_t gx, gy, gz;
float Kp, Ki, Kd;
float Te, Pe;
float alpha;
float angle, adjusted, AdjustedAngle;
long tm, aa, ga;
unsigned long gt, at, t;
int s, p;
long tt;
char Buffer[256];
int Input;
int tail;


float PID(float ang, float dis)
{
  float err = dis - ang;
  Te = Te + err;
  if (Te > 30)
    Te = 30;
  if (Te < -30)
    Te = -30;
  float x = Kp * err + Ki * Te + Kd * (err - Pe);
  Pe = err;
  return x;
}

//Read accel and gyro and fuze with complimentry filter
float getAngle(float td)
{
//  long aa, ga;
  
  imu.getAcceleration(&ax, &ay, &az);
  imu.getGyro(&gx, &gy, &gz);
  aa = ay;
  aa = aa * 90 / 16384;
  if (aa > 90)
    aa = 90;
  if (aa < -90)
    aa = -90;
  ga = gx;
  ga = ga / 131;
  angle = (1-alpha) * (angle + ga * td) + alpha * aa;
  return angle;
}

// TIMER 3 : STEPPER MOTORS SPEED CONTROL
ISR(TIMER3_COMPA_vect)
{
  Mot.LeftStep();
  Mot.RightStep();
}

void doInterface()
{
  int i, j;
  float f;
  char V[10];
  
  Serial1.write('P');Serial1.print(Kp);Serial1.write(',');
  Serial1.write('I');Serial1.print(Ki);Serial1.write(',');
  Serial1.write('D');Serial1.print(Kd);Serial1.write(',');
  Serial1.write('A');Serial1.print(angle);Serial1.write(',');
  Serial1.write('S');Serial1.print(s);Serial1.write(',');
  Serial1.write('X');Serial1.print(Input);Serial1.write(',');
  i = 0;
  j = 0;
  while (i < tail)
  {
    if (Buffer[i] == ',')
    {
      V[j] = 0;
      f = atof(&V[1]);
      j = (int)f;
      switch (V[0])
      {
        case 'P': Kp = f;
            break;
        case 'I': Ki = f;
            break;
        case 'D': Kd = f;
            break;
        case 'X': Input = j;
            break;
      }
      j = 0;
    }
    else
    {
      V[j++] = Buffer[i];
    }
    i++;
  }
  tail = 0;
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial1.setTimeout(250);
  delay(5000);

  Mot.begin(LEFTPIN, LEFTDIR, RIGHTPIN, RIGHTDIR, ENABLED);
  
  int i;
  i = imu.getDeviceID();
  if (i != 0x68)
  {
    Serial.print("Device: ");Serial.println(i, HEX);
    Serial.print("Device not found!");
    while (true) {delay(1000);}
  }
  
  imu.initialize();

  imu.getAccelOffset(&ax, &ay, &az);
  imu.getGyroOffset(&gx, &gy, &gz);

  Serial.print("Accel Offset: ");
  Serial.print(ax);Serial.print(", ");Serial.print(ay);Serial.print(", ");Serial.println(az);
  Serial.print("Gyro Offset: ");
  Serial.print(gx);Serial.print(", ");Serial.print(gy);Serial.print(", ");Serial.println(gz);

  doInit();

  Serial.println("WiFi Startup");
  
  Esp.begin();
  Serial.println("Connecting...");
  Esp.doConnect(WiFi, Auth);

//  S.attach(10, 700, 2300);
//  S.writeMicroseconds(1500);
  
  i = 0;  // set to one to not wait for wifi
  while (i == 0)
  {
    i = Esp.isConnected();
    if (i == 1)
    {
      if (Esp.doTransparent("9750"))
        Serial.println("Transparent Mode");
    }
  }

  // Prime the pump
  for (i=0;i<50;i++)
  {
    angle = getAngle(1);
    adjusted = -PID(angle, 0);
    delay(10);
  }
  Serial.println(angle);
  
  // Output headings for use with Excel
  Serial.println("Accel, Gyro, Angle, PID");
}

void loop() {
  tm = millis();
  if (tm >= gt)
  {
    angle = getAngle(.01);  //get reading every 10 milliseconds
    gt = tm + 10;
    AdjustedAngle = angle + 3;  //adjust for hardware issues
    adjusted = -PID(AdjustedAngle,Input); //adjust angle based on PID and target angle
    if (abs(adjusted) > 45)  //shut it down if tipped over
      adjusted = 0;
//    adjusted = angle;  //just balance based on angle only
    adjusted = adjusted * PI/180;
    s = adjusted * 150; //how many pulses are needed per angle
    Mot.LeftStep(s);
    Mot.RightStep(s);
  }

  if (tm >= t)
  {
    Serial.print(aa);Serial.print(", ");Serial.print(ga);Serial.print(", ");Serial.print(angle);
    Serial.print(", ");Serial.println(adjusted);
    t = tm + 250;
//    S.writeMicroseconds(1500+Input*10);
    doInterface();
  }

  if (Serial1.available() > 0)
  {
    while (Serial1.available() > 0)
    {
      char c = Serial1.read();
      Buffer[tail++] = c;
    }
  }
}

void doInit()
{
  at = millis() + 5;
  gt = millis() + 10;
  alpha = .02;
  angle = 0;
  imu.setAccelOffset(-2258, -1928, 1274);
  imu.setGyroOffset(32, -58, 14);
  tt = 0;
  Mot.LeftStep(0);
  Mot.RightStep(0);
  Kp = .5;
  Ki = .4;
  Kd = .5;
  Input = 0;
  tail = 0;
  
  OCR3A = 80;
  TCCR3A = 0;
  TCCR3B = (1 << WGM32) | (1 << CS31);
  TCNT3 = 0;
  TIMSK3 |= (1 << OCIE1A);
  OCR3A = 40;
}

