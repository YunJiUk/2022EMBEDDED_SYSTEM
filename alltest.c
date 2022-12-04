#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include <sys/msg.h>

#include "led.h"
#include "button.h"
#include "buzzer.h"
#include "fnd.h"
#include "lcdtext.h"
#include "colorled.h"
#include "temperature.h"
#define INPUT_DEVICE_LIST "/dev/input/event"
//실제 디바이스 드라이버 노드파일: 뒤에 숫자가 붙음., ex)/dev/input/event5
#define PROBE_FILE "/proc/bus/input/devices"
//PPT에 제시된 "이 파일을 까보면 event? 의 숫자를 알수 있다"는 바로 그 파일

#define MESSAGE_ID 1122

int msgID2;
BUTTON_MSG_T MSR;
pthread_t buttonread;

void* buttonReadFunc(void *arg)
{ buttonInit();
    msgID2 = msgget (MESSAGE_ID, IPC_CREAT|0666);
while(1){
    
    

    msgrcv(msgID2,&MSR, sizeof(int)*2 , 0,0);
    if(MSR.keyInput){
    switch(MSR.keyInput)
    {
        case KEY_VOLUMEUP: printf("Volume up key):"); break;
        case KEY_HOME: printf("Home key):"); break;
        case KEY_SEARCH: printf("Search key):"); break;
        case KEY_BACK: printf("Back key):"); break;
        case KEY_MENU: printf("Menu key):"); break;
        case KEY_VOLUMEDOWN: printf("Volume down key):"); break;
    }

    if ( MSR.pressed ) printf("pressed\n");
    else printf("released\n");
        }
    } 
    buttonExit();
    }

int main(void){
    pthread_create(&buttonread, NULL, buttonReadFunc, NULL);
   
    ledread("0x01");
   //ledread(argv[1]);
    sleep(1);
    ledread("0x02");
    sleep(1);
    ledread("0x04");
    sleep(1);
   ledLibExit();
    sleep(1);
    
    buzzerInit();

    buzzerPlaySong(1);
    usleep(500000);
    buzzerPlaySong(2);
    usleep(500000);
    buzzerPlaySong(3);
    usleep(500000);
    buzzerStopSong();

    buzzerExit();

    fndDisp(887766 , 0b010101);
    sleep(3);
    disappear();

    lcdtextwrite("1st line write", "Don't write", 1);  //11
    sleep(1);
    lcdtextwrite("Don't write", "2nd line write", 2);  //44
    sleep(1);
    lcdtextwrite("Both write 1", "Both write 2", 3);  //5566

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

tempread();
    sleep(1);
    tempread();
    sleep(1);
    tempread();
    sleep(1);



   pthread_join(buttonread, NULL);
}
