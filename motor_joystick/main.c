#include <stdio.h>
#include <softPwm.h>
#include <wiringPi.h>

#define MOTOR_PIN_1 0 // GP17 -> L293D:7
#define MOTOR_PIN_2 2 // GP27 -> L293D:2
#define MOTOR_ENABLE 3 // GP22 -> L293D:1
#define ADC_CS 6 // GP25 -> ADC0834:2
#define ADC_CLK 4 // GP23 -> ADC0834:12
#define ADC_DIO 5 // GP24 -> ADC0834:[13,10]
#define PWM_RANGE 100
#define ANALOG_RANGE 256

// L293D
// 1|12EN_Vcc1|16
// 2|1A_____4A|15
// 3|1Y_____4Y|14
// 4|HS&G_HS&G|13
// 5|HS&G_HS&G|12
// 6|2Y_____3Y|11
// 7|2A_____3A|10
// 8|Vcc2_34EN|9

// ADC0834
// 1|V+____VCC|14
// 2|CS_____DI|13
// 3|CH0___CLK|12
// 4|CH1__SARS|11
// 5|CH2____DO|10
// 6|CH3___REF|9
// 7|DGND_AGND|8

typedef unsigned char uchar;
typedef unsigned int uint;

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
  uchar adc0, adc1;
  if (wiringPiSetup() ==
      -1) { // when initialize wiring failed,print messageto screen
    printf("setup wiringPi failed !");
    return 1;
  }
  softPwmCreate(MOTOR_PIN_1, 0, PWM_RANGE);
  softPwmCreate(MOTOR_PIN_2, 0, PWM_RANGE);
  pinMode(MOTOR_ENABLE, OUTPUT);
  pinMode(ADC_CS, OUTPUT);
  pinMode(ADC_CLK, OUTPUT);

  // of the ANALOG_RANGE, how much middle defines the off-region
  int mid_size = 30;
  int low_size = (ANALOG_RANGE - mid_size) / 2;
  int high_size = ANALOG_RANGE - mid_size - low_size;
  int high_start = low_size + mid_size;
  int mid_start = low_size;
  int high_max_val = high_size - 1;
  int low_max_val = low_size - 1;

  while (1) {
    adc0 = get_ADC_Result(0);
    adc1 = get_ADC_Result(1);
    printf("Current adc: %d, %d\n", adc0, adc1);
    if (adc0 >= high_start) {
      digitalWrite(MOTOR_ENABLE, HIGH);
      int val = (adc0 - high_start) * PWM_RANGE / high_max_val;
      softPwmWrite(MOTOR_PIN_1, 0);
      softPwmWrite(MOTOR_PIN_2, val);
    }else if(adc0 >= mid_start){
      digitalWrite(MOTOR_ENABLE, LOW);
      softPwmWrite(MOTOR_PIN_1, LOW);
      softPwmWrite(MOTOR_PIN_2, 0);
    }else {
      digitalWrite(MOTOR_ENABLE, HIGH);
      int val = (low_max_val - adc0) * PWM_RANGE / low_max_val;
      softPwmWrite(MOTOR_PIN_1, val);
      softPwmWrite(MOTOR_PIN_2, 0);
    }
    // set servo pwm
    {
      softPwmWrite(SERVO)
      
    }
    delay(100);
  }
  return 0;
}
