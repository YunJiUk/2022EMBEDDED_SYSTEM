#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include <sys/msg.h>
#include "button.h"


// first read input device
#define INPUT_DEVICE_LIST "/dev/input/event"
//실제 디바이스 드라이버 노드파일: 뒤에 숫자가 붙음., ex)/dev/input/event5
#define PROBE_FILE "/proc/bus/input/devices"
//PPT에 제시된 "이 파일을 까보면 event? 의 숫자를 알수 있다"는 바로 그 파일

#define MESSAGE_ID 1122

int msgID2;
BUTTON_MSG_T MSR;

int main(int argc, char *argv[])
{
    buttonInit();
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
