
#include "MPU6050.h"
#include <Wire.h>

MPU6050::MPU6050()
{
  Wire.begin();
  // I2C 400Khz fast mode
  TWSR = 0;
  TWBR = ((16000000L / 400000L) - 16) / 2;
  TWCR = 1 << TWEN;
}

// set mpu to default setting
void MPU6050::initialize()
{
    setPowerManagement1(MPU6050_CLOCK_PLL_XGYRO);
    setGyroRange(MPU6050_GYRO_FS_250);
    setAccelRange(MPU6050_ACCEL_FS_2);
}

// enable interrupts on pin
void MPU6050::setInterruptMode(uint8_t mode)
{
   writeByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_INT_PIN_CFG, mode);
}

// enable interrupts 
void MPU6050::setIntEnabled(uint8_t enabled)
{
  writeByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_INT_ENABLE, enabled);
}

// get interrupt status flag
uint8_t MPU6050::getIntStatus()
{
  uint8_t b;
  
    b = readByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_INT_STATUS);
    return b;
}

// set the accelerometer range
void MPU6050::setAccelRange(uint8_t range)
{
  range = range << 3;
  writeByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_ACCEL_CONFIG, range);
}

// get the current accelerometer values
void MPU6050::getAcceleration(int16_t* x, int16_t* y, int16_t* z)
{
  uint8_t b[6];
  
    readBytes(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_ACCEL_XOUT_H, 6, b);
    *x = (((int16_t)b[0]) << 8) | b[1];
    *y = (((int16_t)b[2]) << 8) | b[3];
    *z = (((int16_t)b[4]) << 8) | b[5];
}

// get accelerometer offsets
void MPU6050::getAccelOffset(int16_t* x, int16_t* y, int16_t* z)
{
  uint8_t b[6];
  
  readBytes(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_XA_OFFS_H, 6, b);
    *x = (((int16_t)b[0]) << 8) | b[1];
    *y = (((int16_t)b[2]) << 8) | b[3];
    *z = (((int16_t)b[4]) << 8) | b[5];
}

// set accelerometer offsets
void MPU6050::setAccelOffset(int16_t x, int16_t y, int16_t z)
{
  uint8_t b;

  b = x >> 8;
  writeByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_XA_OFFS_H, b);
  b = x & 0xff;
  writeByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_XA_OFFS_L_TC, b);
  b = y >> 8;
  writeByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_YA_OFFS_H, b);
  b = y & 0xff;
  writeByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_YA_OFFS_L_TC, b);
  b = z >> 8;
  writeByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_ZA_OFFS_H, b);
  b = z & 0xff;
  writeByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_ZA_OFFS_L_TC, b);
}

// set gyro range
void MPU6050::setGyroRange(uint8_t range)
{
  range = range << 3;
  writeByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_GYRO_CONFIG, range);
}

// get current gyro values
void MPU6050::getGyro(int16_t* x, int16_t* y, int16_t* z)
{
  uint8_t b[6];
  
    readBytes(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_GYRO_XOUT_H, 6, b);
    *x = (((int16_t)b[0]) << 8) | b[1];
    *y = (((int16_t)b[2]) << 8) | b[3];
    *z = (((int16_t)b[4]) << 8) | b[5];
}

// get gyro offsets
void MPU6050::getGyroOffset(int16_t* x, int16_t* y, int16_t* z)
{
  uint8_t b[6];

  readBytes(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_XG_OFFS_USRH, 6, b);
  *x = (((int16_t)b[0]) << 8) | b[1];
  *y = (((int16_t)b[2]) << 8) | b[3];
  *z = (((int16_t)b[4]) << 8) | b[5];
}

// set gyro offsets
void MPU6050::setGyroOffset(int16_t x, int16_t y, int16_t z)
{
  uint8_t b;

  b = x >> 8;
  writeByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_XG_OFFS_USRH, b);
  b = x & 0xff;
  writeByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_XG_OFFS_USRL, b);
  b = y >> 8;
  writeByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_YG_OFFS_USRH, b);
  b = y & 0xff;
  writeByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_YG_OFFS_USRL, b);
  b = z >> 8;
  writeByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_ZG_OFFS_USRH, b);
  b = z & 0xff;
  writeByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_ZG_OFFS_USRL, b);  
}

// get internal temperature of imu
int16_t MPU6050::getTemperature()
{
  uint8_t b[2];
  
    readBytes(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_TEMP_OUT_H, 2, b);
    return (((int16_t)b[0]) << 8) | b[1];
}

// set power management
void MPU6050::setPowerManagement1(uint8_t power)
{
    writeByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_PWR_MGMT_1, power);
}

// get who am i value
uint8_t MPU6050::getDeviceID()
{
  uint8_t b;
  
    b = readByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_WHO_AM_I);
    return b;
}

// get imu current sample rate
uint8_t MPU6050::getSampleRate()
{
  uint8_t b;
  
    b = readByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_SMPLRT_DIV);
    return b;  
}

// set imu sample rate
void MPU6050::setSampleRate(uint8_t rate)
{
  writeByte(MPU6050_DEFAULT_ADDRESS, MPU6050_RA_SMPLRT_DIV, rate);
}

// Wire.h read and write protocols
void MPU6050::writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
  Wire.beginTransmission(address);  // Initialize the Tx buffer
  Wire.write(subAddress);           // Put slave register address in Tx buffer
  Wire.write(data);                 // Put data in Tx buffer
  Wire.endTransmission();           // Send the Tx buffer
}

uint8_t MPU6050::readByte(uint8_t address, uint8_t subAddress)
{
  uint8_t data;                            // "data" will store the register data   
  Wire.beginTransmission(address);         // Initialize the Tx buffer
  Wire.write(subAddress);                  // Put slave register address in Tx buffer
  Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
  Wire.requestFrom(address, (uint8_t) 1);  // Read one byte from slave register address 
  data = Wire.read();                      // Fill Rx buffer with result
  return data;                             // Return data read from slave register
}

void MPU6050::readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest)
{  
  Wire.beginTransmission(address);   // Initialize the Tx buffer
  Wire.write(subAddress);            // Put slave register address in Tx buffer
  Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
  uint8_t i = 0;
  Wire.requestFrom(address, count);  // Read bytes from slave register address 
  while (Wire.available())           // Put read results in the Rx buffer
  {
    dest[i++] = Wire.read();
  }
}


