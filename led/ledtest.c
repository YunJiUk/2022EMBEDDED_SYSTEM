#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "led.h"
#define LED_DRIVER_NAME "/dev/periled"


void doHelp(void)
{
printf("ledtest <hex byte> :data bit0 operation 1=>on 0=>off\n");
printf(" ledtest 0x05 ;4th and 1th led on\n");
printf(" ledtest 0xff ;all led on\n");
printf(" ledtest 0x00 ;all led off\n");
}

int main(int argc , char **argv){
	unsigned int data = 0;
	int fd;
	
	if (argc < 2 ){
		perror(" Args number is less than 2\n");
		doHelp();
		return 1;
	}
	
	ledLibInit();
	
	while(1){
    ledread("0x01");
   //ledread(argv[1]);
    sleep(1);
    ledread("0x02");
    sleep(1);
    ledread("0x04");
    sleep(1);
   ledLibExit();
    sleep(1);
    }
	
	
	return 0;
}
