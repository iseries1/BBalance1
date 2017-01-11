// Stepper motor driver using A4988
// Developer Michael Burmeister
// Date December 30, 2016
// 

#include "A4988.h"

A4988::A4988()
{
  L = 0;
  R = 0;
  Rs = 400;
  Ls = 400;
}

void A4988::begin(uint8_t Left, uint8_t LeftDirection, uint8_t Right, uint8_t RightDirection, uint8_t Enable)
{
  LeftPin = Left;
  LeftDir = LeftDirection;
  RightPin = Right;
  RightDir = RightDirection;
  Enabled = Enable;
  pinMode(LeftPin, OUTPUT);
  pinMode(LeftDir, OUTPUT);
  pinMode(RightPin, OUTPUT);
  pinMode(RightDir, OUTPUT);
  pinMode(Enabled, OUTPUT);
}

// 16 single cycle instructions = 1us at 16Mhz
void A4988::delay_1us()
{
  __asm__ __volatile__ (
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop");
}

// Turn motors on/off
void A4988::Enable(bool en)
{
  if (en)
    digitalWrite(Enabled, LOW);
  else
    digitalWrite(Enabled, HIGH);
}

// Step the motor x units in one direction
// direction positive is forward
void A4988::LeftStep(int16_t number)
{
  if (number > 0)
    digitalWrite(LeftDir, LOW);
  else
    digitalWrite(LeftDir, HIGH);
  if (number == 0)
  {
    L = 0;
    return;
  }
  L = abs(number);
  L = L * 11;
  L = 100000/L;
}

void A4988::LeftStep()
{
  if (micros() < Ls)
    return;
  if (L == 0)
    return;
  digitalWrite(LeftPin, HIGH);
  delay_1us();
  digitalWrite(LeftPin, LOW);
  Ls = micros() + L;
}

void A4988::RightStep()
{
  if (micros() < Rs)
    return;
  if (R == 0)
    return;
  digitalWrite(RightPin, HIGH);
  delay_1us();
  digitalWrite(RightPin, LOW);
  Rs = micros() + R;
}

// Step the motor x units in one direction
// direct positive is forward
void A4988::RightStep(int16_t number)
{
  if (number > 0)
    digitalWrite(RightDir, HIGH);
  else
    digitalWrite(RightDir, LOW);
  if (number == 0)
  {
    R = 0;
    return;
  }
  R = abs(number);
  R = R * 11;
  R = 100000/R;
}


