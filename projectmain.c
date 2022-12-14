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
#include "touch.h"
#include "gyro.h"
#include "jpgAnimation.h"

#define INPUT_DEVICE_LIST "/dev/input/event"
//실제 디바이스 드라이버 노드파일: 뒤에 숫자가 붙음., ex)/dev/input/event5
#define PROBE_FILE "/proc/bus/input/devices"
//PPT에 제시된 "이 파일을 까보면 event? 의 숫자를 알수 있다"는 바로 그 파일

#define MESSAGE_ID 1122

int msgID2;
BUTTON_MSG_T MSR;
pthread_t thread_led;
pthread_t thread_buttonread;
pthread_t thread_fnd;
pthread_t thread_temperature;
pthread_t thread_buzzer;
pthread_t thread_colorled;
pthread_t thread_touch;
pthread_t thread_game1;
pthread_t thread_game2;
pthread_t thread_main_TFT;
pthread_t thread_happy_TFT;
pthread_t thread_backtomain_TFT;
pthread_t thread_hunger_decrease;
pthread_t thread_maintain;

pthread_mutex_t lock;

int friendship_exp=0;
int hunger_scale=0;
unsigned int animation_flag=0;

void* maintainFunc(){
    while(1){
    if(hunger_scale<0){
        hunger_scale=0;
    }
    if(hunger_scale>100){
        hunger_scale=100;
    }
    if(friendship_exp<0){
        friendship_exp=0;
    }
    }
}
void* hungerDecreaseFunc(){
    while(1){
    if(hunger_scale>0){
        hunger_scale--;
    }

   
    if(hunger_scale<30)
    {
        friendship_exp-=10;
    }
    sleep(1);
    }
}


void* animationFunc(){
    pthread_mutex_lock(&lock);
    
    if(pthread_self()==thread_main_TFT)
    {  
        AnimationInit();
        while(animation_flag==0) AnimationPrint_2("./lobby/lobby",5);
        
        AnimationExit();
    }
    else if(pthread_self()==thread_happy_TFT)
    {
        AnimationInit();
        while(animation_flag==1) AnimationPrint_2("./happydog/happy",24);
        
        AnimationExit();
    }
    
    
    pthread_mutex_unlock(&lock);
   
}

void* back(){
    sleep(2);
    animation_flag=0;
    pthread_create(&thread_main_TFT, NULL, animationFunc, NULL);
}


void* game1Func(void *arg)
{ /*
    Gyroinit();
    while(1){
    int x = Gyroscope();
    if(x < -300){
        printf("\nleft\n");
        sleep(1);
    }
    else if(x > 300){
        printf("\nright\n");
        sleep(1);
    }
    }
    Gyroexit(); */
}

void* game2Func(void *arg)
{  
  
}
void* buttonReadFunc(void *arg)
{
    buttonInit();
    msgID2 = msgget (MESSAGE_ID, IPC_CREAT|0666);
while(1){
    
    msgrcv(msgID2,&MSR, sizeof(int)*2 , 0,0);
    if(MSR.keyInput){
    switch(MSR.keyInput)
    {
        case KEY_VOLUMEUP:
            pthread_create(&thread_game1, NULL, game1Func, NULL); 
            break;
        case KEY_HOME: if ( MSR.pressed ) friendship_exp+=5; break;
        case KEY_SEARCH: printf("Search key):"); break;
        case KEY_BACK:  if ( MSR.pressed ) friendship_exp-=5; break;
        case KEY_MENU: printf("Menu key):"); break;
        case KEY_VOLUMEDOWN: 
            pthread_create(&thread_game2, NULL, game2Func, NULL); 
            break;
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
    if(friendship_exp<100) ledread("1");
    else if (friendship_exp<200) ledread("11");
    else if (friendship_exp<300) ledread("111");
    else if (friendship_exp<400) ledread("1111");
    else if (friendship_exp<500) ledread("11111");
    else if (friendship_exp<600) ledread("111111");
    else if (friendship_exp<700) ledread("1111111");
    else if (friendship_exp<800) ledread("11111111");
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
  /* while(1){
    if(tempread()>27){
    friendship_exp=friendship_exp+100;
    animation_flag=1;
    pthread_create(&thread_happy_TFT, NULL, animationFunc, NULL);
    pthread_create(&thread_backtomain_TFT,NULL,back,NULL);
    }
    sleep(30);
    }
    */
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
    if(hunger_scale<30) {
        pwmSetPercent(0, 0);
        pwmSetPercent(0, 1);
        pwmSetPercent(100, 2);  //red
       
    }
    else if ((30<=hunger_scale)&&(hunger_scale<=70)) {
        pwmSetPercent(0, 0);
        pwmSetPercent(100, 1);
        pwmSetPercent(0, 2);  //green
    }
    else if (90<hunger_scale) {
        pwmSetPercent(100, 0);
        pwmSetPercent(0, 1);
        pwmSetPercent(0, 2);  //blue
    }
    
   }
    
}

 void* touchFunc(void *arg)
{   
    touchInit();
    int touchflag=0;
     int msgID_t=msgget (MESSAGE_ID_T, IPC_CREAT|0666);
     TOUCH_MSG_T MR_touch;
    while(1){
       msgrcv(msgID_t,&MR_touch,sizeof(int)*4 , 0,0);
       
       switch(MR_touch.keyInput){
           case 999:
           
            if(MR_touch.pressed==1){
                if(MR_touch.x>35 && MR_touch.x<387 && MR_touch.y>415 && MR_touch.y<539) 
                {
                    if(hunger_scale<=70){
                    printf("give Food\n");
                    friendship_exp+=30;
                    hunger_scale+=30;
                    animation_flag=1;
                    pthread_create(&thread_happy_TFT, NULL, animationFunc, NULL);
                    pthread_create(&thread_backtomain_TFT,NULL,back,NULL);
                    
                    }
                }
                else if(MR_touch.x>815 && MR_touch.x<965 && MR_touch.y>336 && MR_touch.y<533) 
                    {
                    if(hunger_scale<=90){
                    printf("give Snack\n");
                    friendship_exp+=100;
                    hunger_scale+=10;
                    animation_flag=1;
                    pthread_create(&thread_happy_TFT, NULL, animationFunc, NULL);
                    pthread_create(&thread_backtomain_TFT,NULL,back,NULL);
                    
                    }
                }
                
            }
             
       }
       
       
    }
    touchExit();
    
}






int main(void){
    ledLibInit();
    ledread("0");
    ledLibExit();
    fndDisp(0,0);

    
    lcdtextwrite("1: increase", "2. decrease", 3);  
    pthread_create(&thread_hunger_decrease, NULL, hungerDecreaseFunc, NULL);
    pthread_create(&thread_maintain, NULL, maintainFunc, NULL);
    pthread_create(&thread_buttonread, NULL, buttonReadFunc, NULL);
    pthread_create(&thread_led, NULL, ledFunc, NULL);
    pthread_create(&thread_fnd, NULL, fndFunc, NULL);
    pthread_create(&thread_temperature, NULL, temperatureFunc, NULL);
    pthread_create(&thread_buzzer, NULL, buzzerFunc, NULL);
    pthread_create(&thread_colorled, NULL, colorledFunc, NULL);
    pthread_create(&thread_touch, NULL, touchFunc, NULL);
    animation_flag=0;
    pthread_create(&thread_main_TFT, NULL, animationFunc, NULL);
   
  
    
   pthread_join(thread_hunger_decrease, NULL);
   pthread_join(thread_buttonread, NULL);
   pthread_join(thread_led, NULL);
   pthread_join(thread_fnd, NULL);
   pthread_join(thread_temperature, NULL);
   pthread_join(thread_buzzer, NULL);
   pthread_join(thread_colorled, NULL);
   pthread_join(thread_touch,NULL);
   pthread_join(thread_main_TFT,NULL);
}
