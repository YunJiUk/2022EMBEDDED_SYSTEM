#include <sys/ipc.h>
#include <stdio.h>
#include "touch.h"

int main(void){
    
    touchInit();
     int msgID=msgget (MESSAGE_ID_T, IPC_CREAT|0666);
     TOUCH_MSG_T MR_touch;
    while(1){
       msgrcv(msgID,&MR_touch,sizeof(int)*4 , 0,0);
       
       switch(MR_touch.keyInput){
           case 999:
            if(MR_touch.pressed==1){
                if(MR_touch.x<500) printf("left\n");
                else if(MR_touch.x>500) printf("right\n");
            }
       }
    }
    touchExit();
}