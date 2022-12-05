#include <stdint.h>
#include "temperature.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

int main(void){
while(1){
    double x = tempread();
    printf("helloworld %lf", x);
    sleep(1);
    }
}
