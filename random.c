#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

pthread_t thread_object_1;   //스레드 1 선언
pthread_t thread_object_2;   //스레드 2 선언
pthread_mutex_t lockInput;  ////buffer와 ready를 보호하기 위한 mutex 선언
int ready=0;  //입력받았는지 상태를 나타낼 변수 선언
int guessNumber;  //알아맞힐 정수를 나타낼 변수 선언

void* func1(void *arg){  //스레드 1이 실행할 함수
  
    
    while(1){
        srand(time(NULL));
        pthread_mutex_lock(&lockInput);  //lock으로 다른 스레드의 동시 수행 차단
        if(ready==0){  //랜덤 숫자를 입력을 받지 않은 상태라면
    	guessNumber = rand();  //랜덤숫자를 입력받고
		printf("Thread1의 생성 및 출력 %d\n", guessNumber);   //guessNumber 출력
        ready=1;    //현재 상태를 랜덤 숫자를 입력받은 상태로 설정
        pthread_mutex_unlock(&lockInput);  // 다른 스레드가 수행할수 있도록 lock 해제
        usleep(1000000);  //1초 대기
        }
        else
        {
            pthread_mutex_unlock(&lockInput);  // 다른 스레드가 수행할수 있도록 lock 해제
            usleep(1);  //1us 대기
        }
        
    }
}

void* func2(void *arg){  //스레드 2가 실행할 함수
   
    
    while(1){
        pthread_mutex_lock(&lockInput);
        if(ready==1){  //랜덤 숫자를 입력을 받은 상태라면
        
            printf("Thread2의 출력 %d\n", guessNumber);   //guessNumber 출력
            ready=0;  //현재 상태를 랜덤 숫자를 입력받지 않은 상태로 설정
            pthread_mutex_unlock(&lockInput);  // 다른 스레드가 수행할수 있도록 lock 해제
            usleep(1);  //1us 대기
        }
        else
        {
            pthread_mutex_unlock(&lockInput);  // 다른 스레드가 수행할수 있도록 lock 해제
            usleep(1);  //1us 대기
        }
    }
}


int main (void){
    
    int err;  //스레드가 정상적으로 생성되었는지를 확인할 때 사용할 변수
    
    
    if (pthread_mutex_init(&lockInput, NULL) != 0)  //mutex 객체를 초기화
    {
        printf ("\n Mutex Init Failed!!\n");  //실패하면 문구 출력
        return 1;  //1을 리턴하고 프로세스 종료
    }
   
    err = pthread_create(&thread_object_1, NULL,func1, NULL);  //스레드1 생성
        if (err != 0)
        printf ("Thread1 Create Error\n");  //스레드가 생성이 되지 않았을시(err가 0이 아닐 시) 문구 출력
       
    err = pthread_create(&thread_object_2, NULL, func2, NULL);  // //스레드2 생성
        if (err != 0)
        printf ("Thread2 Create Error\n");  //스레드가 생성이 되지 않았을시(err가 0이 아닐 시) 문구 출력
    
    pthread_join (thread_object_1, NULL);  //스레드1 종료 대기
    pthread_join (thread_object_2, NULL);  //스레드2 종료 대기

    return 0;  //0을 리턴하고 프로세스 종료
       
}