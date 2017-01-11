
#ifndef _MPU6050_H_
#define _MPU6050_H_

#include <arduino.h>
#include "MPU6050Reg.h" // hide these values

class MPU6050
{
  public:
    MPU6050();
    void initialize();
    void setInterruptMode(uint8_t mode);
    void setIntEnabled(uint8_t enabled);
    uint8_t getIntStatus();
    void setAccelRange(uint8_t range);
    void getAcceleration(int16_t* x, int16_t* y, int16_t* z);
    void getAccelOffset(int16_t* x, int16_t* y, int16_t* z);
    void setAccelOffset(int16_t x, int16_t y, int16_t z);
    void setGyroRange(uint8_t range);
    void getGyroOffset(int16_t* x, int16_t* y, int16_t* z);
    void setGyroOffset(int16_t x, int16_t y, int16_t z);
    void getGyro(int16_t* x, int16_t* y, int16_t* z);
    int16_t getTemperature();
    void setPowerManagement1(uint8_t power);
    uint8_t getDeviceID();
    uint8_t getSampleRate();
    void setSampleRate(uint8_t rate);

  private:
    void writeByte(uint8_t, uint8_t, uint8_t);
    uint8_t readByte(uint8_t, uint8_t);
    void readBytes(uint8_t, uint8_t, uint8_t, uint8_t *);
};

#endif


