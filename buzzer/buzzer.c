#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "buzzer.h"

#define MAX_SCALE_STEP 12
#define BUZZER_BASE_SYS_PATH "/sys/bus/platform/devices/"
#define BUZZER_FILENAME "peribuzzer"
#define BUZZER_ENABLE_NAME "enable"
#define BUZZER_FREQUENCY_NAME "frequency"
char gBuzzerBaseSysDir[128]; ///sys/bus/platform/devices/peribuzzer.XX 가 결정됨
const int musicScale[MAX_SCALE_STEP] =
{
262, 278, 294,311, 330,349,370, 392,415,440,466, 494
};
const int musicScale2[MAX_SCALE_STEP] =
{
523,554,587,622,659,699,740,784,831,880,932,988
};

int fd1, fd2;


//버저 경로 찾기: /sys/bus/platform/devices/peribuzzer.XX 의 XX를 결정하는 것

void doHelp(void)
{
printf("Usage:\n");
printf("buzzertest <buzzerNo> \n");
printf("buzzerNo: \n");
printf("do(1),re(2),mi(3),fa(4),sol(5),ra(6),si(7),do(8) \n");
printf("off(0)\n");
}

void buzzerEnable(int bEnable)
{
char path[200];
sprintf(path,"%s%s",gBuzzerBaseSysDir,BUZZER_ENABLE_NAME);
int fd=open(path,O_WRONLY);
if ( bEnable) write(fd, &"1", 1);
else write(fd, &"0", 1);
close(fd);
}

void setFrequency(int frequency)
{
char path[200];
sprintf(path,"%s%s",gBuzzerBaseSysDir,BUZZER_FREQUENCY_NAME);
int fd=open(path,O_WRONLY);
dprintf(fd, "%d", frequency);
close(fd);
}

int buzzerInit(){
DIR * dir_info = opendir(BUZZER_BASE_SYS_PATH);
int ifNotFound = 1;
if (dir_info != NULL){
    while (1){
    struct dirent *dir_entry;
    dir_entry = readdir (dir_info);
    if (dir_entry == NULL) break;
    if (strncasecmp(BUZZER_FILENAME, dir_entry->d_name, strlen(BUZZER_FILENAME)) == 0){
    ifNotFound = 0;
    sprintf(gBuzzerBaseSysDir,"%s%s/",BUZZER_BASE_SYS_PATH,dir_entry->d_name);
}
}
}
printf("find %s\n",gBuzzerBaseSysDir);
return ifNotFound;
}

int buz(int scale){
    setFrequency(musicScale[scale-1]);
    buzzerEnable(1);
}
int buz2(int scale){
    setFrequency(musicScale2[scale-1]);
    buzzerEnable(1);
}


int buzzerStopSong(void){
    buzzerEnable(0);
}

int buzzerExit(void){
printf("bye~");
}

void game1bgm(void){
int C=1, D1=3, D2 =4, E=5, F1=6, F2 = 7, G=8, A=10, B1= 11, B2=12;//도레미파솔라시도 숫자화
   while(1){
    buzzerInit();
    buz(A);    usleep(250000);
    buz2(C);     usleep(500000);
    buz2(D1);    usleep(250000);
    buz2(C);    usleep(1000000);
    buz(A);    usleep(375000);
    buz2(C);    usleep(125000);    buzzerStopSong();   usleep(100);
    buz2(C);    usleep(250000);    buzzerStopSong();    usleep(100);
    buz2(C);    usleep(250000);    buz2(D1);    usleep(250000);
    buz2(C);    usleep(750000);
    buz(A);    usleep(250000);
    buz2(C);    usleep(500000);    buzzerStopSong();    usleep(100);
    buz2(C);    usleep(250000);
    buz2(D1);    usleep(250000);
    buz2(C);    usleep(250000);
    buz(A);    usleep(250000);
    buz(F1);    usleep(250000);
    buz(G);    usleep(1500000);
    buz(B1);    usleep(250000);    buzzerStopSong();    usleep(100);
    buz(B1);    usleep(250000);    buzzerStopSong();    usleep(100);
    buz(B1);    usleep(250000);
    buz(A);    usleep(250000);
    buz(G);    usleep(250000);
    buz(A);    usleep(250000);
    buz(B1);    usleep(500000);    buzzerStopSong();    usleep(100);
    buz(B1);    usleep(250000);    buzzerStopSong();    usleep(100);
    buz(B1);    usleep(250000);    buzzerStopSong();    usleep(100);
    buz(B1);    usleep(250000);
    buz(A);    usleep(250000);
    buz(G);    usleep(250000);
    buz(A);    usleep(250000);
    buz(B1);    usleep(500000);
    buz(A);    usleep(250000);
    buz2(C);    usleep(500000);
    buz(B1);    usleep(250000);
    buz(A);    usleep(250000);
    buz(G);    usleep(250000);
    buz(F1);    usleep(250000);
    buz(E);    usleep(250000);
    buz(F1);    usleep(1500000);
}   
    buzzerStopSong();
}
void game2bgm(void){
 int C = 1, D1 = 3, D2 = 4, E = 5, F1 = 6, F2 = 7, G = 8, A = 10, B1 = 11, B2 = 12;
    while (1) {

        buzzerInit();
        buz(C);
        usleep(450000);
        buzzerStopSong();
        usleep(50000);
        buz(C);
        usleep(500000);
        buz(G);
        usleep(450000);
        buzzerStopSong();
        usleep(50000);
        buz(G);
        usleep(500000);
        buz(A);
        usleep(450000);
        buzzerStopSong();
        usleep(50000);
        buz(A);
        usleep(500000);
        buz(G);
        usleep(1000000);
        buz(F1);
        usleep(450000);
        buzzerStopSong();
        usleep(50000);
        buz(F1);
        usleep(500000);
        buz(E);
        usleep(450000);
        buzzerStopSong();
        usleep(50000);
        buz(E);
        usleep(500000);
        buz(D1);
        usleep(450000);
        buzzerStopSong();
        usleep(50000);
        buz(D1);
        usleep(500000);
        buz(C);
        usleep(1000000);
        buz(G);
        usleep(450000);
        buzzerStopSong();
        usleep(50000);
        buz(G);
        usleep(500000);
        buz(F1);
        usleep(450000);
        buzzerStopSong();
        usleep(50000);
        buz(F1);
        usleep(500000);
        buz(E);
        usleep(450000);
        buzzerStopSong();
        usleep(50000);
        buz(E);
        usleep(500000);
        buz(D1);
        usleep(1000000);
        buz(G);
        usleep(450000);
        buzzerStopSong();
        usleep(50000);
        buz(G);
        usleep(500000);
        buz(F1);
        usleep(450000);
        buzzerStopSong();
        usleep(50000);
        buz(F1);
        usleep(500000);
        buz(E);
        usleep(450000);
        buzzerStopSong();
        usleep(50000);
        buz(E);
        usleep(500000);
        buz(D1);
        usleep(1000000);
        buz(C);
        usleep(450000);
        buzzerStopSong();
        usleep(50000);
        buz(C);
        usleep(500000);
        buz(G);
        usleep(450000);
        buzzerStopSong();
        usleep(50000);
        buz(G);
        usleep(500000);
        buz(A);
        usleep(450000);
        buzzerStopSong();
        usleep(50000);
        buz(A);
        usleep(500000);
        buz(G);
        usleep(1000000);
        buz(F1);
        usleep(450000);
        buzzerStopSong();
        usleep(50000);
        buz(F1);
        usleep(500000);
        buz(E);
        usleep(450000);
        buzzerStopSong();
        usleep(50000);
        buz(E);
        usleep(500000);
        buz(D1);
        usleep(450000);
        buzzerStopSong();
        usleep(50000);
        buz(D1);
        usleep(500000);
        buz(C);
        usleep(1000000);
        buzzerStopSong();
    }
}
