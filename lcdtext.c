#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "lcdtext.h"

int lcdtextwrite(const char *str1, const char *str2, int lineFlag){
    stTextLCD stlcd; // stTextLCD 구조체를 가지고 드라이버와 인터페이스
    int fd;
    int len;
    memset(&stlcd,0,sizeof(stTextLCD)); // 구조체 초기화
    printf("lineFlag :%d\n", lineFlag);

    int write1(){
        stlcd.cmdData = CMD_DATA_WRITE_LINE_1;
        len = strlen(str1);
        if ( len > COLUMN_NUM)
        memcpy(stlcd.TextData[stlcd.cmdData - 1], str1, COLUMN_NUM);
    else
        memcpy(stlcd.TextData[stlcd.cmdData - 1], str1, len);
        stlcd.cmd = CMD_WRITE_STRING;
        fd = open(TEXTLCD_DRIVER_NAME,O_RDWR); // open driver
    if ( fd < 0 ) {
        perror("driver (/dev/peritextlcd) open error.\n");
        return 1;
    }
    write(fd,&stlcd,sizeof(stTextLCD));
    close(fd);
    printf("   %s\n", str1);
     }
     int write2(){
        stlcd.cmdData = CMD_DATA_WRITE_LINE_2;
        len = strlen(str2);
        if ( len > COLUMN_NUM)
        memcpy(stlcd.TextData[stlcd.cmdData - 1], str2, COLUMN_NUM);
    else
        memcpy(stlcd.TextData[stlcd.cmdData - 1], str2, len);
        stlcd.cmd = CMD_WRITE_STRING;
        fd = open(TEXTLCD_DRIVER_NAME,O_RDWR); // open driver
    if ( fd < 0 ) {
        perror("driver (/dev/peritextlcd) open error.\n");
        return 1;
    }
    write(fd,&stlcd,sizeof(stTextLCD));
    close(fd);
    printf("   %s\n", str2);
    }

     if ( (lineFlag) == 1) write1();

     else if ((lineFlag) == 2) write2();

      else if ((lineFlag) == 3)
      {
        write1();
        write2();
      }

}
