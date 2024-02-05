#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

// UART pins are
// GPIO 14 TXD (wiring pi 15)
// GPIO 15 RXD (wiring pi 16)

void start(int fd)
{
  serialPutchar(fd, 128);
}

void reset(int fd)
{
  serialPutchar(fd, 7);
}

void stop(int fd)
{
  serialPutchar(fd, 173);
}

int main()
{
  int fd;
  unsigned int nextTime;

  // Roomba Open Interface (OI)
  // https://cdn-shop.adafruit.com/datasheets/create_2_Open_Interface_Spec.pdf
  // Baud: 115200 (default) or 19200
  // Data bits: 8
  // Parity: None
  // Stop bits: 1
  // Flow control: None
  // default for wiringSerial is 8N1
  if ((fd = serialOpen("/dev/ttyAMA0", 115200)) < 0)
  {
    fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
    return 1;
  }

  if (wiringPiSetup() == -1)
  {
    fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
    return 1;
  }

  printf("start \n");
  start(fd);
  delay(3);
  printf("stop \n");
  stop(fd);
  if (0)
  {
    while (serialDataAvail(fd))
    {
      printf(" -> %3d", serialGetchar(fd));
    }
  }

  printf("done \n");
  return 0;
}
