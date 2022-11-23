#ifndef _LED_H_
#define _LED_H_

int ledInit(void);
int ledOnOff (int ledNum, int onOff);
int ledStatus (void);
int ledExit(void);
int ledread(const char* data);

#define LED_DRIVER_NAME "/dev/periled"

#endif //LED_H_
