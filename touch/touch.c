#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include <sys/msg.h>
#include <pthread.h>
#include "touch.h"
// first read input device
#define INPUT_DEVICE_LIST_T "/dev/input/event" //실제 디바이스 드라이버 노드파일: 뒤에 숫자가 붙음., ex)/dev/input/event5
#define PROBE_FILE_T "/proc/bus/input/devices" //PPT에 제시된 "이 파일을 까보면 event? 의 숫자를 알수 있다"는 바로 그 파일
#define HAVE_TO_FIND_T_1 "N: Name=\"WaveShare WaveShare Touchscreen\"\n"
#define HAVE_TO_FIND_T_2 "H: Handlers=mouse0 event"


int fd_t;
TOUCH_MSG_T MS_touch;
pthread_t touchTh_id;
char touchPath[200] = {0,};
int msgID_t;

int probeTouchPath(char *newPath){
	int returnValue = 0; //touch에 해당하는 event#을 찾았나?
	int number = 0; //찾았다면 여기에 집어넣자
	FILE *fp = fopen(PROBE_FILE_T,"rt");
	while(!feof(fp)) {
	char tmpStr[200]; //200자를 읽을 수 있게 버퍼
	fgets(tmpStr,200,fp); //최대 200자를 읽어봄
	//printf ("%s",tmpStr);
	if (strcmp(tmpStr,HAVE_TO_FIND_T_1) == 0){
		printf("YES! I found!: %s\r\n", tmpStr);
		returnValue = 1; //찾음
	}
	
	if ((returnValue == 1) && (strncasecmp(tmpStr, HAVE_TO_FIND_T_2, strlen(HAVE_TO_FIND_T_2)) == 0) )
	{
	printf ("-->%s",tmpStr);
	printf("\t%c\r\n",tmpStr[strlen(tmpStr)-3]);
	number = tmpStr[strlen(tmpStr)-3] - '0';
	//Ascii character '0'-'9' (0x30-0x39)
	//to interger(0)
	break;
	}
	}
	fclose(fp);
	if (returnValue == 1){
		sprintf (newPath,"%s%d",INPUT_DEVICE_LIST_T,number);
	}
	
	return returnValue;
}
void* touchThFunc(void *arg)
{
	int readSize,inputIndex;
	int x=0;
	int y=0;
	
	struct input_event stEvent;
	while(1)
	{
	readSize = read(fd_t, &stEvent , sizeof(stEvent));
	
	if (readSize != sizeof(stEvent))
	{
	continue;
	}
	if(stEvent.type ==EV_ABS){
			if(stEvent.code==ABS_MT_POSITION_X)
			{
				x=stEvent.value;
				//printf("touch X: %d\r\n",stEvent.value);
			}
			else if(stEvent.code==ABS_MT_POSITION_Y)
			{
				y=stEvent.value;
				//printf("touch Y: %d\r\n",stEvent.value);
			}
	}
	else if((stEvent.type==EV_KEY)&&(stEvent.code==BTN_TOUCH)){
		MS_touch.x=x;
		MS_touch.y=y;

		if(stEvent.value==0){
			MS_touch.pressed=0;
			//printf("touch finished\n");
		}
		else if(stEvent.value==1){
			MS_touch.pressed=1;
			//printf("touch started\n");
			}
	}

	MS_touch.messageNum=3;
	
	MS_touch.keyInput = 999;
	MS_touch.pressed = stEvent.value;
	
	
	msgsnd(msgID_t,&MS_touch, sizeof(int)*4 , 0);
	
	
	
	}
  
}
int touchInit(void)
{
	
if (probeTouchPath(touchPath) == 0) 
return 0;
fd_t=open (touchPath, O_RDWR);
msgID_t = msgget (MESSAGE_ID_T, IPC_CREAT|0666);
pthread_create(&touchTh_id, NULL, touchThFunc, NULL);

return 1;
}

int touchExit(void)
{
	pthread_join(touchTh_id, NULL);
	
	close(fd_t);

	return 1;
}


