/**********************************************************************
 * Filename    : 1.3.1_Motor.c
 * Description : Controlling a motor.
 * Author      : Robot
 * E-mail      : support@sunfounder.com
 * website     : www.sunfounder.com
 * Update      : Daisy    2019/08/05
 **********************************************************************/
#include <stdio.h>
#include <wiringPi.h>

#define MotorPin1 0
#define MotorPin2 2
#define MotorEnable 3

int main(void) {
  int i;
  if (wiringPiSetup() ==
      -1) { // when initialize wiring failed, print message to screen
    printf("setup wiringPi failed !");
    return 1;
  }

  pinMode(MotorPin1, OUTPUT);
  pinMode(MotorPin2, OUTPUT);
  pinMode(MotorEnable, OUTPUT);

  while (1) {
    printf("Clockwise\n");
    delay(100);
    digitalWrite(MotorEnable, HIGH);
    digitalWrite(MotorPin1, HIGH);
    digitalWrite(MotorPin2, LOW);
    for (i = 0; i < 3; i++) {
      delay(1000);
    }

    printf("Stop\n");
    delay(100);
    digitalWrite(MotorEnable, LOW);
    for (i = 0; i < 3; i++) {
      delay(1000);
    }

    printf("Anti-clockwise\n");
    delay(100);
    digitalWrite(MotorEnable, HIGH);
    digitalWrite(MotorPin1, LOW);
    digitalWrite(MotorPin2, HIGH);
    for (i = 0; i < 3; i++) {
      delay(1000);
    }

    printf("Stop\n");
    delay(100);
    digitalWrite(MotorEnable, LOW);
    for (i = 0; i < 3; i++) {
      delay(1000);
    }
  }
  return 0;
}

/**********************************************************************
 * Filename    : 2.1.4_Potentiometer.c
 * Description :
 * Author      : Robot
 * E-mail      : support@sunfounder.com
 * website     : www.sunfounder.com
 * Update      : Daisy    2019/07/29
 **********************************************************************/
#include <softPwm.h>
#include <stdio.h>
#include <wiringPi.h>

typedef unsigned char uchar;
typedef unsigned int uint;

#define ADC_CS 0
#define ADC_CLK 1
#define ADC_DIO 2
#define LedPin 3

uchar get_ADC_Result(uint channel) {
  uchar i;
  uchar dat1 = 0, dat2 = 0;
  int sel = channel > 1 & 1;
  int odd = channel & 1;

  pinMode(ADC_DIO, OUTPUT);
  digitalWrite(ADC_CS, 0);
  // Start bit
  digitalWrite(ADC_CLK, 0);
  digitalWrite(ADC_DIO, 1);
  delayMicroseconds(2);
  digitalWrite(ADC_CLK, 1);
  delayMicroseconds(2);
  // Single End mode
  digitalWrite(ADC_CLK, 0);
  digitalWrite(ADC_DIO, 1);
  delayMicroseconds(2);
  digitalWrite(ADC_CLK, 1);
  delayMicroseconds(2);
  // ODD
  digitalWrite(ADC_CLK, 0);
  digitalWrite(ADC_DIO, odd);
  delayMicroseconds(2);
  digitalWrite(ADC_CLK, 1);
  delayMicroseconds(2);
  // Select
  digitalWrite(ADC_CLK, 0);
  digitalWrite(ADC_DIO, sel);
  delayMicroseconds(2);
  digitalWrite(ADC_CLK, 1);

  delayMicroseconds(2);
  digitalWrite(ADC_CLK, 0);
  delayMicroseconds(2);

  for (i = 0; i < 8; i++) {
    digitalWrite(ADC_CLK, 1);
    delayMicroseconds(2);
    digitalWrite(ADC_CLK, 0);
    delayMicroseconds(2);

    pinMode(ADC_DIO, INPUT);
    dat1 = dat1 << 1 | digitalRead(ADC_DIO);
  }

  for (i = 0; i < 8; i++) {
    dat2 = dat2 | ((uchar)(digitalRead(ADC_DIO)) << i);
    digitalWrite(ADC_CLK, 1);
    delayMicroseconds(2);
    digitalWrite(ADC_CLK, 0);
    delayMicroseconds(2);
  }

  digitalWrite(ADC_CS, 1);
  pinMode(ADC_DIO, OUTPUT);
  return (dat1 == dat2) ? dat1 : 0;
}

int main(void) {
  uchar analogVal;
  if (wiringPiSetup() ==
      -1) { // when initialize wiring failed,print messageto screen
    printf("setup wiringPi failed !");
    return 1;
  }
  softPwmCreate(LedPin, 0, 100);
  pinMode(ADC_CS, OUTPUT);
  pinMode(ADC_CLK, OUTPUT);

  while (1) {
    analogVal = get_ADC_Result(0);
    printf("Current analogVal : %d\n", analogVal);
    softPwmWrite(LedPin, analogVal);
    delay(100);
  }
  return 0;
}