#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "colorled.h"
#define COLOR_LED_DEV_R_ "/sys/class/pwm/pwmchip0/"
#define COLOR_LED_DEV_G_ "/sys/class/pwm/pwmchip1/"
#define COLOR_LED_DEV_B_ "/sys/class/pwm/pwmchip2/"
#define PWM_EXPORT "export"
#define PWM_UNEXPORT "unexport"
#define PWM_DUTY "pwm0/duty_cycle"
#define PWM_PERIOD "pwm0/period"
#define PWM_ENABLE "pwm0/enable"
#define PWM_COLOR_R 0
#define PWM_COLOR_G 1
#define PWM_COLOR_B 2

#define PWM_PERIOD_NS 1000000 //ns. = 1ms = 1khz



int main(int argc, char *argv[]) {

int B=0;
int G=0;
int R=0;

pwmLedInit();

B=100;
G=0;
R=0;

pwmSetPercent(B, 0);
pwmSetPercent(G, 1);
pwmSetPercent(R, 2);
sleep(1);

B=0;
G=100;
R=0;

pwmSetPercent(B, 0);
pwmSetPercent(G, 1);
pwmSetPercent(R, 2);
sleep(1);

B=0;
G=0;
R=100;

pwmSetPercent(B, 0);
pwmSetPercent(G, 1);
pwmSetPercent(R, 2);
sleep(1);

pwmInactiveAll();
return 0;
}


