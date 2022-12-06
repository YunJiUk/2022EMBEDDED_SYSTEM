#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "gyro.h"
#define ACCELPATH "/sys/class/misc/FreescaleAccelerometer/"
#define MAGNEPATH "/sys/class/misc/FreescaleMagnetometer/"
#define GYROPATH "/sys/class/misc/FreescaleGyroscope/"

int fd = 0;
FILE *fp_Acc = NULL;
FILE *fp_Mag = NULL;
FILE *fp_Gyro = NULL;

void Gyroinit(){
fd = open(ACCELPATH "enable",O_WRONLY);
dprintf (fd,"1");
close(fd);

fd = open (MAGNEPATH "enable",O_WRONLY);
dprintf (fd,"1");
close(fd);

fd = open (GYROPATH "enable",O_WRONLY);
dprintf (fd,"1");
close(fd);
}

int Accelerometer(){
fp_Acc = fopen (ACCELPATH "data", "rt");
int accel[3];
fscanf(fp_Acc,"%d, %d, %d",&accel[0],&accel[1],&accel[2]);
printf ("I read Accel %d, %d, %d\r\n",accel[0],accel[1],accel[2]);
fclose(fp_Acc);
return accel[0];
}

int Magnetometer(){
fp_Mag = fopen (MAGNEPATH "data", "rt");
int magne[3];
fscanf(fp_Mag,"%d, %d, %d",&magne[0],&magne[1],&magne[2]);
printf ("I read Magneto %d, %d, %d\r\n",magne[0],magne[1],magne[2]);
fclose(fp_Mag);
return magne[0];
}

int Gyroscope(){
fp_Gyro = fopen (GYROPATH "data", "rt");
int gyro[3];
fscanf(fp_Gyro,"%d, %d, %d",&gyro[0],&gyro[1],&gyro[2]);
printf ("I read Gyroscope %d, %d, %d\r\n",gyro[0],gyro[1],gyro[2]);
fclose(fp_Gyro);
return gyro[0];
}

void Gyroexit(){
fd = open(ACCELPATH "enable",O_WRONLY);
dprintf (fd,"0");
close(fd);

fd = open (MAGNEPATH "enable",O_WRONLY);
dprintf (fd,"0");
close(fd);

fd = open (GYROPATH "enable",O_WRONLY);
dprintf (fd,"0");
close(fd);
}
