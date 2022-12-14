#ifndef _JPGANIMATION_H_
#define _JPGANIMATION_H_

#define GAME2MSG_ID 7777

typedef struct
{
long int messageType_jpg;
int pressThisButton;
} structgame2Msg;




int AnimationInit(void);

int AnimationPrint(char *jpg,int count_jpg);

int AnimationPrint_2(char *jpg,int count_jpg);

int AnimationPrint_delay(char *jpg, int numberOfJpg);

int AnimationExit(void);

	
#endif