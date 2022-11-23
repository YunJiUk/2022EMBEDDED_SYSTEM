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

#define MAX_LED_NUM 8
#define LED_DRIVER_NAME "/dev/periled"

static unsigned int ledValue = 0;
static int fd = 0;
unsigned int data=0;


int ledOnOff (int ledNum, int onOff){
    
    int i=1;
	i = i<<ledNum;
	ledValue = ledValue& (~i);
	
	if (onOff !=0) ledValue |= i;
	
    write (fd, &ledValue, 4);
}

int ledLibInit(void){

	fd=open("/dev/periled", O_WRONLY);
	
	ledValue = 0;
}

int ledLibExit(void)
{
    ledValue = 0;
    ledOnOff (0, 0);
    close(fd);
}

int ledread(const char num[]){

   fd=open("/dev/periled", O_WRONLY);
if ( fd < 0 )
{
perror("driver (//dev//cnled) open error.\n");
return 1;
}
	data = strtol(num,NULL,16);
	printf("wrate data :0x%X\n", data);
	write(fd,&data,4);
}

int ledStatus(void){
	
	return ledValue;
}
