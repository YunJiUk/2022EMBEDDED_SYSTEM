#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include <sys/msg.h>
#include <pthread.h>
#include <dirent.h>
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
#include "../libfbdev/libfbdev.h"
#include "../libjpeg/jpeglib.h"



int msgID2;
BUTTON_MSG_T MSR;
pthread_t thread_led;
pthread_t thread_buttonread;
pthread_t thread_fnd;
pthread_t thread_temperature;
pthread_t thread_colorled;
pthread_t thread_touch;
pthread_t thread_main_TFT;
pthread_t thread_happy_TFT;
pthread_t thread_backtomain_TFT;
pthread_t thread_hunger_decrease;
pthread_t thread_hunger_cooldown;
pthread_t thread_maintain;
pthread_t thread_buzzer_happy;

pthread_mutex_t lock;

int friendship_exp=0;  //호감도 수치
int hunger_scale=0;    //포만감 수치
unsigned int animation_flag=0;    //애니메이션 전환을 위해 사용하는 플래그
int hunger_cooldown=0;   //포만감을 올리는 데 쿨타임을 두기 위해 사용하는 플래그

////Game1에 필요한 선언
int screen_width_01;
int screen_height_01;
int bits_per_pixel_01;
int line_length_01;
int cols_01 = 0, rows_01 = 0;
char *data_01;
int loseflag_01 = 0;

pthread_t thread_walk_TFT;
pthread_t thread_game1;

//Game2에 필요한 선언
int sense1timeflag=0;
int sense2timeflag=0;
int sense3timeflag=0;
int sense4timeflag=0;
int game2button_received=0;

int game2_button=0;   //목욕게임 중 버튼 입력을 저장할 변수
int game2_start=0;   //목욕게임이 실행중인지를 알려주는 플래그
int bath_score=0;   //목욕게임 점수

pthread_t thread_game2;
pthread_t thread_bath_TFT;
pthread_t sense1;
pthread_t sense2;
pthread_t sense3;
pthread_t sense4;
pthread_t sense1timeout;
pthread_t sense2timeout;
pthread_t sense3timeout;
pthread_t sense4timeout;
pthread_t thread_game2bgm;

void* animationFunc() {     //애니메이션 출력 플래그
    pthread_mutex_lock(&lock);     //mutex lock으로 2개 이상의 쓰레드의 접근을 막아
                                    //segmentation error 방지

    if (pthread_self() == thread_main_TFT)
    {
        AnimationInit();
        while (animation_flag == 0) AnimationPrint_2("./lobby/lobby", 5); //로비화면 출력

        AnimationExit();
    }
    else if (pthread_self() == thread_happy_TFT)
    {
        AnimationInit();
        while (animation_flag == 1) AnimationPrint_2("./happydog/happy", 24);  //펫의 반응 애니메이션 출력

        AnimationExit();
    }
    else if (pthread_self() == thread_bath_TFT)
    {
        AnimationInit();
        AnimationPrint_bath("./bath/bath", 47);   //목욕게임 애니메이션 출력
        AnimationExit();
    }

    else if (pthread_self() == thread_walk_TFT)
    {
        int game1clear = 0;
        AnimationInit();
        game1clear = AnimationPrint_walk();    //산책게임 애니메이션 출력
        if (game1clear == 1) {          //게임 승리시 호감도 증가
            friendship_exp += 500;
        }
        AnimationExit();
    }

    pthread_mutex_unlock(&lock);

}

void* back() {    //로비 애니메이션으로 돌아가는 함수
    sleep(2);
    animation_flag = 0;   //플래그 다시 0으로 -> 로비화면으로 돌아갈 준비
    pthread_create(&thread_main_TFT, NULL, animationFunc, NULL);  //로비화면 애니메이션 출력
    sleep(2);
    lcdtextwrite("Select game!", "1: walk 2: bath", 3);  //로비 화면 안내문구 출력
}

void* game1Func() {  //산책게임 실행 함수(산책 게임를 동작시키는 자세한 코드는 jpgAnimation.c 내부에 존재)
    loseflag_01 = 0;
    animation_flag = 3;   //애니메이션 플래그 3으로-> animationFunc내의 while문 탈출 

    pthread_create(&thread_walk_TFT, NULL, animationFunc, NULL);   //산책게임 애니메이션 출력
    pthread_join(thread_walk_TFT, NULL);   //쓰레드 종료 대기
    pthread_create(&thread_backtomain_TFT, NULL, back, NULL);    //다시 로비화면으로

}

void* sense1timeoutFunc(){  //목욕게임을 위한 함수, 1.5초를 대기하는 함수
    usleep(1500000);
    sense1timeflag=1;
}
void* sense2timeoutFunc(){
    usleep(1500000);
    sense2timeflag=1;
}
void* sense3timeoutFunc(){
    usleep(1500000);
    sense3timeflag=1;
}
void* sense4timeoutFunc(){
    usleep(1500000);
    sense4timeflag=1;
}

void* sense1Func(void *arg) //목욕게임을 위한 함수, 버튼을 인식하기 위한 함수
{   


    pthread_create(&sense1timeout, NULL, sense1timeoutFunc, NULL);  //1.5초 대기하는 함수를 실행시키는 쓰레드
    while(1){
        if(game2_button==1){   // //1.5초 안에 버튼을 누른 경우
            bath_score++;   //점수 증가
            printf("%d\n",bath_score);
            game2_button=0;
            pthread_cancel(sense1timeout);  ////1.5초 대기하는 쓰레드 종료
            break;
        }
        if(sense1timeflag==1) {  //1.5초 안에 버튼을 누르지 못한 경우
            sense1timeflag=0;



            break;
        }
    }
}

void* sense2Func(void *arg)
{   


    pthread_create(&sense2timeout, NULL, sense2timeoutFunc, NULL);
    while(1){
        if(game2_button==2){
            bath_score++;
            printf("%d\n",bath_score);
            game2_button=0;
            pthread_cancel(sense2timeout);
            break;
        }
        if(sense2timeflag==1) {
            sense2timeflag=0;
           


            break;
        }
    }
}

void* sense3Func(void *arg)
{   
 

    pthread_create(&sense3timeout, NULL, sense3timeoutFunc, NULL);
    while(1){
        if(game2_button==3){
            bath_score++;
            printf("%d\n",bath_score);
            game2_button=0;
            pthread_cancel(sense3timeout);
            break;
        }
        if(sense3timeflag==1) {
            sense3timeflag=0;
        


            break;
        }
    }
}

void* sense4Func(void *arg)
{   
  

    pthread_create(&sense4timeout, NULL, sense4timeoutFunc, NULL);
    while(1){
        if(game2_button==4){
            bath_score++;
            printf("%d\n",bath_score);
            game2_button=0;
            pthread_cancel(sense4timeout);
            break;
        }
        if(sense4timeflag==1) {
            sense4timeflag=0;
           


            break;
        }
    }
}

void* game2bgmFunc() {
    game2bgm();
}


void* game2Func(void* arg)  //목욕게임을 실행시키는 함수
{
    lcdtextwrite("Search:1 Menu:2", "V.UP:3 V.DN:4", 3);  //text LCD에 설명 표시

    game2_start = 1;    //목욕게임 플래그를 1로
    animation_flag = 2;   //애니메이션 플래그를 2로 ->while문 탈출
    pthread_create(&thread_bath_TFT, NULL, animationFunc, NULL);    //목욕게임 애니메이션 출력
    pthread_create(&thread_game2bgm, NULL, game2bgmFunc, NULL);    //bgm 출력
    printf("game 1\n");
    structgame2Msg Game2RxData;

    while (1) {
        int Game2msgQueue = msgget((key_t)GAME2MSG_ID, IPC_CREAT | 0666);
        msgrcv(Game2msgQueue, &Game2RxData, 4, 0, IPC_NOWAIT);  //메시지큐를 이용해 jpcAnimation.c에서 어떤 버튼을 눌러야하는지를 수신
        game2button_received = Game2RxData.pressThisButton;
        Game2RxData.pressThisButton = 0;
        if (game2button_received == 1) {       //메시지 큐로 어떤 버튼을 눌러야하는지를 수신한 후 그 버튼을 인식하기 위한 쓰레드 생성
            pthread_create(&sense1, NULL, sense1Func, NULL);

        }
        else if (game2button_received == 2) {
            pthread_create(&sense2, NULL, sense2Func, NULL);

        }
        else if (game2button_received == 3) {
            pthread_create(&sense3, NULL, sense3Func, NULL);

        }
        else if (game2button_received == 4) {
            pthread_create(&sense4, NULL, sense4Func, NULL);

        }
        else if (game2button_received == 111) break;     //111-> 게임이 끝났다는 신호

    }

    game2_start = 0;     //목욕게임 플래그 다시 0으로
    char game2score[10];   //점수를 text LCD에 출력하기 위한 배열 선언

    pthread_cancel(thread_game2bgm);   //bgm 종료
    buzzerStopSong();
    sprintf(game2score, "%d", bath_score);
    lcdtextwrite("Your Score is", game2score, 3);     //text LCD에 점수 출력
    usleep(500000);
    lcdtextwrite(" ", " ", 3);
    lcdtextwrite("Your Score is", game2score, 3);
    usleep(500000);
    lcdtextwrite(" ", " ", 3);
    lcdtextwrite("Your Score is", game2score, 3);

    if (bath_score < 10) friendship_exp -= 100;       //10점 미만 -> 호감도 감소
    else friendship_exp += 300;     //10점 이상 -> 호감도 증가

    pthread_create(&thread_backtomain_TFT, NULL, back, NULL); // 다시 로비 화면으로

        bath_score = 0;   //점수 초기화
}


void* maintainFunc(){   //포만감이 0~100 사이의 값, 호감도가 양수의 값을 벗어나지 않도록 유지
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

void* hungerCooldownFunc(){   //포만감을 올리는데에 쿨타임을 부여하는 함수
    while(1){
    if(hunger_cooldown==1){
        sleep(5);
        hunger_cooldown=0;
    }
    }


}
void* hungerDecreaseFunc(){  //주기적으로 포만감 및 호감도 감소
    while(1){
    if(hunger_scale>0){
        hunger_scale--;
    }

   
    if(hunger_scale<30)
    {
        friendship_exp-=10;
    }
    sleep(5);
    }
}

void* happybuzzerFunc(){  //펫의 반응 효과음 쓰레드 함수
    buzzerInit();
                        
     buz(1);
     usleep(300000);
     buz(3);
       usleep(300000);
       buz(5);
       usleep(300000);
      buz(8);
       usleep(300000);                
     buzzerStopSong();
      buzzerExit();
}






void* buttonReadFunc(void *arg)     //버튼 쓰레드
{
    buttonInit();
    msgID2 = msgget (MESSAGE_ID, IPC_CREAT|0666);
while(1){
    
    msgrcv(msgID2,&MSR, sizeof(int)*2 , 0,0);  //메시지큐를 이용해 버튼 입력 수신
    if(MSR.keyInput){
    switch(MSR.keyInput)
    {
        case KEY_HOME:
        if ( MSR.pressed ){     //Home 버튼이 눌렸을 경우
            if(game2_start==0){    //목욕게임이 실행중이지 않다면
            pthread_create(&thread_game1, NULL, game1Func, NULL);    //산책게임 쓰레드 create
            break;
            }
            
        }
        case KEY_BACK: if ( MSR.pressed ) {  //Back 버튼이 눌렸을 경우
            if(game2_start==0){     //목욕게임이 실행중이지 않다면
            pthread_create(&thread_game2, NULL, game2Func, NULL);      //목욕게임 쓰레드 create
            break;
            }
        }
        case KEY_SEARCH: if ( MSR.pressed ){
             if(game2_start==1){    //목욕게임이 실행중이라면
                    game2_button=1;
                      
             }

            break;
        }
        case KEY_MENU:  if ( MSR.pressed )  {  //Menu 버튼이 눌렸을 경우
             if(game2_start==1){    //목욕게임이 실행중이라면
                    game2_button=2;
                     
             }
                break;
            }
        case KEY_VOLUMEUP: if ( MSR.pressed )  {  //Vol up 버튼이 눌렸을 경우
             if(game2_start==1){    //목욕게임이 실행중이라면
                    game2_button=3;
                    
             }
                 break;
        }
        case KEY_VOLUMEDOWN: if ( MSR.pressed )  {   //Vol down 버튼이 눌렸을 경우
             if(game2_start==1){      //목욕게임이 실행중이라면
                    game2_button=4;
                     
             }
            break;
        }
    }

    
        }
    } 
    buttonExit();
    }

    void* ledFunc(void *arg)   //LED 쓰레드
{   
    ledLibInit();
    while(1){
    if(friendship_exp<200) ledread("1");    //호감도 수치에 따라 켜진 LED 개수 조절
    else if (friendship_exp<400) ledread("11");
    else if (friendship_exp<600) ledread("111");
    else if (friendship_exp<800) ledread("1111");
    else if (friendship_exp<1000) ledread("11111");
    else if (friendship_exp<1200) ledread("111111");
    else if (friendship_exp<1400) ledread("1111111");
    else if (friendship_exp<1600) ledread("11111111");
    }
    ledLibExit();
}

    void* fndFunc(void *arg)   //FND 쓰레드
{   
    while(1){
    fndDisp(friendship_exp , 0);   //FND에 호감도 표기
    }
}
   void* temperatureFunc(void *arg)  //온도센서 쓰레드
{   
   while(1){
    if(tempread()>30){    //온도가 30도 이상이면
    lcdtextwrite("pet me more!", "",3);  //문구 출력
    friendship_exp=friendship_exp+100;   //호감도 증가
    animation_flag=1;    //애니메이션 플래그 1로 -> 애니메이션 쓰레드 내의 while문 탈출
    pthread_create(&thread_buzzer_happy,NULL,happybuzzerFunc,NULL);   //효과음 출력
    pthread_create(&thread_happy_TFT, NULL, animationFunc, NULL);    //TFT-LCD에 반응 출력
    pthread_create(&thread_backtomain_TFT,NULL,back,NULL);     //다시 로비로
    }
    sleep(60);    //60초 대기
    }
    
}


 void* colorledFunc(void *arg)
{   
pwmLedInit();
    
   while(1){
    if(hunger_scale<30) {
        pwmSetPercent(0, 0);
        pwmSetPercent(0, 1);
        pwmSetPercent(100, 2);  //포만감 수치가 30미만이면 red
    }
    else if ((30<=hunger_scale)&&(hunger_scale<=80)) {
        pwmSetPercent(0, 0);
        pwmSetPercent(100, 1);
        pwmSetPercent(0, 2);  //포만감 수치가 30이상 80이하면 green
    }
    else if (80<hunger_scale) {
        pwmSetPercent(100, 0);
        pwmSetPercent(0, 1);
        pwmSetPercent(0, 2);  //포만감 수치가 80초과이면 blue
    }
    
   }
    
}

 void* touchFunc(void *arg)    //터치입력을 받는 쓰레드
{   
    touchInit();
    int touchflag=0;
     int msgID_t=msgget (MESSAGE_ID_T, IPC_CREAT|0666);
     TOUCH_MSG_T MR_touch;
    while(1){
       msgrcv(msgID_t,&MR_touch,sizeof(int)*4 , 0,0);  //메시지큐를 이용해 터치 입력 수신
       
       switch(MR_touch.keyInput){
           case 999:
           
            if(MR_touch.pressed==1){  //터치가 인식되었을 시
                if(MR_touch.x>35 && MR_touch.x<387 && MR_touch.y>415 && MR_touch.y<539) //사료의 좌표라면
                {   
                    if(hunger_cooldown==0){     //음식을 줄 수 있는 쿨타임이 돌아왔다면
                        if(hunger_scale<=70){   //포만감 수치가 70 이하라면
                        printf("give Food\n");
                        lcdtextwrite("yummy!", "",3);  
                        pthread_create(&thread_buzzer_happy,NULL,happybuzzerFunc,NULL);  //효과음 출력
                        friendship_exp+=30;
                        hunger_scale+=30;
                        animation_flag=1;
                        pthread_create(&thread_happy_TFT, NULL, animationFunc, NULL);   //TFT-LCD에 반응 출력
                        pthread_create(&thread_backtomain_TFT,NULL,back,NULL);  //다시 로비로
                        hunger_cooldown=1;    //쿨타임 플래그 1로

                        

                        }
                    
                    }
                    
                }
                else if(MR_touch.x>815 && MR_touch.x<965 && MR_touch.y>336 && MR_touch.y<533)  //간식의 좌표라면
                    {
                    if(hunger_cooldown==0){    //음식을 줄 수 있는 쿨타임이 돌아왔다면
                        if(hunger_scale<=90){      //포만감 수치가 90 이하라면
                        printf("give Snack\n");
                        lcdtextwrite("sweeeeeet!", "",3);  
                        pthread_create(&thread_buzzer_happy,NULL,happybuzzerFunc,NULL);   //효과음 출력
                        friendship_exp+=100;
                        hunger_scale+=10;
                        animation_flag=1;
                        pthread_create(&thread_happy_TFT, NULL, animationFunc, NULL);   //TFT-LCD에 반응 출력
                        pthread_create(&thread_backtomain_TFT,NULL,back,NULL);   //다시 로비로
                        hunger_cooldown=1;    //쿨타임 플래그 1로
                        }
                        
                    }
                    
                }
                
            }
             
       }
       
       
    }
    touchExit();
    
}


int main(void){
    ledLibInit();
    ledread("0");  //LED 초기화
    ledLibExit();
    fndDisp(0,0);   //FND 초기화
    
    lcdtextwrite("Select game!","1: walk 2: bath", 3);    //text LCD에 안내 문구 출력
    pthread_create(&thread_hunger_decrease, NULL, hungerDecreaseFunc, NULL);  //필요한 쓰레드들 create
    pthread_create(&thread_hunger_cooldown, NULL, hungerCooldownFunc, NULL);
    pthread_create(&thread_maintain, NULL, maintainFunc, NULL);
    pthread_create(&thread_buttonread, NULL, buttonReadFunc, NULL);
    pthread_create(&thread_led, NULL, ledFunc, NULL);
    pthread_create(&thread_fnd, NULL, fndFunc, NULL);
    pthread_create(&thread_temperature, NULL, temperatureFunc, NULL);
    pthread_create(&thread_colorled, NULL, colorledFunc, NULL);
    pthread_create(&thread_touch, NULL, touchFunc, NULL);
    animation_flag=0;
    pthread_create(&thread_main_TFT, NULL, animationFunc, NULL);
   
  
    pthread_join(thread_maintain, NULL);   //main문은 종료하지 않고 대기
}