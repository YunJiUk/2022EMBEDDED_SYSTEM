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
pthread_t thread_led;
pthread_t buttonread;
pthread_t thread_fnd;
pthread_t thread_temperature;
pthread_t thread_buzzer;
pthread_t thread_colorled;
unsigned int friendship_exp=0;

void* buttonReadFunc(void *arg)
{
    buttonInit();
    msgID2 = msgget (MESSAGE_ID, IPC_CREAT|0666);
while(1){
    
    msgrcv(msgID2,&MSR, sizeof(int)*2 , 0,0);
    if(MSR.keyInput){
    switch(MSR.keyInput)
    {
        case KEY_VOLUMEUP: printf("Volume up key):"); break;
        case KEY_HOME: if ( MSR.pressed ) friendship_exp++; break;
        case KEY_SEARCH: printf("Search key):"); break;
        case KEY_BACK:  if ( MSR.pressed ) friendship_exp--; break;
        case KEY_MENU: printf("Menu key):"); break;
        case KEY_VOLUMEDOWN: printf("Volume down key):"); break;
    }

    if ( MSR.pressed ) printf("pressed\n");
    else printf("released\n");
        }
    } 
    buttonExit();
    }

    void* ledFunc(void *arg)
{   
    ledLibInit();
    while(1){
    if(friendship_exp<5) ledread("1");
    else if (friendship_exp<10) ledread("11");
    else if (friendship_exp<15) ledread("111");
    else if (friendship_exp<20) ledread("1111");
    else if (friendship_exp<25) ledread("11111");
    else if (friendship_exp<30) ledread("111111");
    else if (friendship_exp<35) ledread("1111111");
    else if (friendship_exp<40) ledread("11111111");
    }
    ledLibExit();
}

    void* fndFunc(void *arg)
{   
    while(1){
    fndDisp(friendship_exp , 0);
    }
}
   void* temperatureFunc(void *arg)
{   
   
    
}

 void* buzzerFunc(void *arg)
{   
   /*while(1){
       buzzerInit();

    

    buzzerExit();
   }*/
    
}

 void* colorledFunc(void *arg)
{   
pwmLedInit();

   while(1){
    if(friendship_exp<5) {
        pwmSetPercent(0, 0);
        pwmSetPercent(100, 1);
        pwmSetPercent(0, 2);
    }
    else if (friendship_exp<10) {
        pwmSetPercent(0, 0);
        pwmSetPercent(0, 1);
        pwmSetPercent(100, 2);
    }
    else if (friendship_exp<15) {
        pwmSetPercent(100, 0);
        pwmSetPercent(0, 1);
        pwmSetPercent(0, 2);
    }
    
   }
    
}




int main(void){

    lcdtextwrite("1: increase", "2. decrease", 3);  

    pthread_create(&buttonread, NULL, buttonReadFunc, NULL);
    pthread_create(&thread_led, NULL, ledFunc, NULL);
    pthread_create(&thread_fnd, NULL, fndFunc, NULL);
    pthread_create(&thread_temperature, NULL, temperatureFunc, NULL);
    pthread_create(&thread_buzzer, NULL, buzzerFunc, NULL);
     pthread_create(&thread_colorled, NULL, colorledFunc, NULL);
   
    



   pthread_join(buttonread, NULL);
   pthread_join(thread_led, NULL);
   pthread_join(thread_fnd, NULL);
   pthread_join(thread_temperature, NULL);
   pthread_join(thread_buzzer, NULL);
   pthread_join(thread_colorled, NULL);
}
