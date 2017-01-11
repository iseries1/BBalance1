
#include "MPU6050.h"
#include "A4988.h"

#define LEFTPIN 7
#define RIGHTPIN 12
#define LEFTDIR 8
#define RIGHTDIR 5
#define ENABLED 4


MPU6050 imu;
A4988 Mot;

int16_t ax, ay, az;
int16_t gx, gy, gz;
float Kp, Ki, Kd;
float Te, Pe;
float alpha;
float angle, adjusted;
long tm, aa, ga;
unsigned long gt, at, t;
int s, p;
long tt;


float PID(float ang, float dis)
{
  float err = dis - ang;
  Te = (Te + err) / 2;
  float x = Kp * err + Ki * Te + Kd * (err - Pe);
  Pe = err;
  return x;
}

float getAngle(float td)
{
//  long aa, ga;
  
  imu.getAcceleration(&ax, &ay, &az);
  imu.getGyro(&gx, &gy, &gz);
  aa = ay;
  aa = aa * 1000 / 16384;
  aa = aa * 90 / 1000;
  if (aa > 90)
    aa = 90;
  if (aa < -90)
    aa = -90;
  ga = gx;
  ga = ga / 131;
  angle = (1-alpha) * (angle + ga * td) + alpha * aa;
  return angle;
}


// TIMER 1 : STEPPER MOTORS SPEED CONTROL
ISR(TIMER1_COMPA_vect)
{
  Mot.LeftStep();
  Mot.RightStep();
}

void setup() {
  Serial.begin(115200);
  Mot.begin(LEFTPIN, LEFTDIR, RIGHTPIN, RIGHTDIR, ENABLED);
  
  delay(5000);
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

  at = millis() + 5;
  gt = millis() + 10;
  alpha = .02;
  angle = 0;
//  imu.setAccelOffset(-2335, -1840, 1255);
  imu.setGyroOffset(35, -50, 20);
  tt = 0;
  Mot.LeftStep(0);
  Mot.RightStep(0);
  OCR1A = 80;
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS11);
  TCNT1 = 0;
  TIMSK1 |= (1 << OCIE1A);
  OCR1A = 40;
  Kp = .9;
  Ki = .5;
  Kd = .5;
  Serial.println("Accel, Gyro, PID, Angle");
}

void loop() {
  tm = millis();
  if (tm >= gt)
  {
    angle = getAngle(.01);  //get reading every 10 milliseconds
    gt = millis() + 10;

    if (angle > 45)
      angle = 45;
    if (angle < -45)
      angle = -45;
    adjusted = -PID(angle,0); //adjust angle based on PID and target angle
//    adjusted = angle;  //just balance based on angle only
    adjusted = adjusted * 3.1415926/180;  //convert to radians
    adjusted = sin(adjusted);
    s = adjusted * 150; //how many pulses are needed per angle
    Mot.LeftStep(s);
    Mot.RightStep(s);
  }

  if (millis() >= t)
  {
    p = -PID(angle, 0);
    Serial.print(aa);Serial.print(", ");Serial.print(ga);Serial.print(", ");Serial.print(p);
    Serial.print(", ");Serial.println(angle);
    t = millis() + 250;
  }
}
