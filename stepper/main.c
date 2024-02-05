#include <stdio.h>
#include <wiringPi.h>

const int motorPin[] = {1, 4, 5, 6};
int rpm = 200;
int stepsPerRevolution = 100;
int stepSpeed = 0;

void rotary(char direction){
    if(direction == 'c'){
        digitalWrite(motorPin[0],0);
        digitalWrite(motorPin[1],1);
        digitalWrite(motorPin[2],1);
        digitalWrite(motorPin[3],0);
        delayMicroseconds(stepSpeed);

        digitalWrite(motorPin[0],1);
        digitalWrite(motorPin[1],0);
        digitalWrite(motorPin[2],1);
        digitalWrite(motorPin[3],0);
        delayMicroseconds(stepSpeed);

        digitalWrite(motorPin[0],1);
        digitalWrite(motorPin[1],0);
        digitalWrite(motorPin[2],0);
        digitalWrite(motorPin[3],1);
        delayMicroseconds(stepSpeed);

        digitalWrite(motorPin[0],0);
        digitalWrite(motorPin[1],1);
        digitalWrite(motorPin[2],0);
        digitalWrite(motorPin[3],1);
        delayMicroseconds(stepSpeed);
    }
    else if(direction =='a'){
        digitalWrite(motorPin[0],0);
        digitalWrite(motorPin[1],0);
        digitalWrite(motorPin[2],0);
        digitalWrite(motorPin[3],0);
        delayMicroseconds(stepSpeed * 10);

        digitalWrite(motorPin[0],0);
        digitalWrite(motorPin[1],0);
        digitalWrite(motorPin[2],0);
        digitalWrite(motorPin[3],0);
        delayMicroseconds(stepSpeed * 10);

        digitalWrite(motorPin[0],0);
        digitalWrite(motorPin[1],0);
        digitalWrite(motorPin[2],0);
        digitalWrite(motorPin[3],0);
        delayMicroseconds(stepSpeed * 10);

        digitalWrite(motorPin[0],0);
        digitalWrite(motorPin[1],0);
        digitalWrite(motorPin[2],0);
        digitalWrite(motorPin[3],0);
        delayMicroseconds(stepSpeed * 10);
    }
}

void loop()
{
    char direction = '0';
    while (1)
    {       
        printf("select motor direction a=anticlockwise, c=clockwise: ");
        delay(100);
        direction=getchar();
        if (direction == 'c')
        {
            printf("motor running clockwise\n");
            delay(100);
            break;
        }
        else if (direction == 'a')
        {
            printf("motor running anti-clockwise\n");
            delay(100);
            break;
        }
        else
        {
            printf("input error, please try again!\n");
        }
    }
    while(1)
    {
        rotary(direction);
    }
}

void main(void)
{
    if (wiringPiSetup() == -1)
    {
        printf("setup wiringPi failed !");
        return;
    }
    for (int i = 0; i < 4; i++)
    {
        pinMode(motorPin[i], OUTPUT);
    }
    stepSpeed = (60000000 / rpm) / stepsPerRevolution;
    loop();
}
