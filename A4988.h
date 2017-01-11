// Stepper Motor Driver using A4988
// Developer Michael Burmeister
// Date December 30, 2016
// Version 1.0
//

#include <Arduino.h>

#ifndef _A4988_H_
#define _A4988_H_

class A4988 {
  private:
  uint8_t LeftPin;
  uint8_t LeftDir;
  uint8_t RightPin;
  uint8_t RightDir;
  uint8_t Enabled;
  void delay_1us(void);
  
  public:
  A4988();
  void begin(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
  void Enable(bool);
  void LeftStep(int16_t);
  void LeftStep();
  void RightStep(int16_t);
  void RightStep();
  unsigned long R, L;
  unsigned long Rs, Ls;
};

#endif
