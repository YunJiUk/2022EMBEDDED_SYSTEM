#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "../libfbdev/libfbdev.h"
#include "../libjpeg/jpeglib.h"
#include <sys/msg.h>
#include "jpgAnimation.h"
#include "gyro.h"
#include "buzzer.h"

pthread_t thread_buzzer; //buzzer thread 선언

void *buzzerFunc() // 산책 게임중에 나오는 배경음악 쓰레드
{   
   game1bgm();
}

void gamewin() //게임 승리시 나오는 브금
{
    buzzerInit();
    buz2(A);
    usleep(350000);
    buz2(F1);
    usleep(350000);
    buz2(C);
    usleep(350000);
    buz2(B1);
    usleep(150000);
    buzzerStopSong(); 
    usleep(50000);
    buz2(B1);
    usleep(150000);
    buzzerStopSong(); 
    usleep(50000);
    buz2(B1);
    usleep(150000);
    buzzerStopSong(); 
    usleep(50000);
    buz2(G);
    usleep(300000);
    buz2(B1);
    usleep(200000);
    buz2(A);
    usleep(500000);
    buzzerStopSong();    
}

void gamelose()//게임 패배시 나오는 브금
{
    buzzerInit();
    buz2(E);
    usleep(160000);
    buz2(D2);
    usleep(160000);
    buz2(E);
    usleep(160000);
    buz2(D2);
    usleep(160000);
    buz2(E);
    usleep(160000);
    buz(B2);
    usleep(160000);
    buz2(D1);
    usleep(160000);
    buz2(C);
    usleep(160000);
    buz(A);
    usleep(160000);
    buzzerStopSong(); 
}

pthread_t thread_gyro; // gyro thread 선언

int space =2; // 강아지의 위치를 나타내줄 변수

void *gyroFunc() //기울기 쓰레드-센서에 감지값 따라 산책게임에서 강아지의 위치를 정해줌
{
    Gyroinit();
    structgame1Msg Game1TxData;
    while(1){
    int x = Accelerometer();
    printf("%d", x);
    if(x > 4500) //4500 이상이면 위치(space)를 증가시킴
    {
        printf("down");
        space++;
        if(space>2){
         space = 3;
      }
    }
    else if(x < -1000) // -1000 이하면 위치(space)를 감소시킴
    {
        printf("up");
      space--;
      if(space<2){
         space = 1;
      }
    }


   sleep(1);
    }
    Gyroexit();
}

    int screen_width;
    int screen_height;
    int bits_per_pixel;
    int line_length;
    int cols = 0, rows = 0;
   int loseflag_01;
   char *data_ani;
   char *data_01;
   
   
   int AnimationInit(){
   //FrameBuffer init
    if ( fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0 )
   {
      printf ("FrameBuffer Init Failed\r\n");
      return 0;
   }
   //Clear FB.
   fb_clear();
   }

int AnimationPrint_walk() // 산책게임
{
   lcdtextwrite("RunRunRunRunRun", "Tilt to Avoid!", 3);
   loseflag_01 = 0;
   int i=0;
   pthread_create(&thread_gyro,NULL,gyroFunc,NULL); // 기울기 쓰레드 실행
   pthread_create(&thread_buzzer,NULL,buzzerFunc,NULL); //배경음악 쓰레드 실행
    for (i=0; i<=19;i++)
   {
     
   char filename_01[200]={0,};
      
   snprintf(filename_01,200,"./rundog/run_%d_%d.JPG",space, i); // 위치, 상황에 맞는 사진 출력
      //FileRead
      int error_01=0;
      struct jpeg_decompress_struct cinfo_01;
      struct jpeg_error_mgr jerr_01;
       cinfo_01.err = jpeg_std_error(&jerr_01);
         jpeg_create_decompress(&cinfo_01);
      FILE *fp_01 = fopen(filename_01, "rb");
       jpeg_stdio_src(&cinfo_01, fp_01);
      jpeg_read_header(&cinfo_01, TRUE); 
      
      cols = cinfo_01.image_width;
      rows = cinfo_01.image_height;

      data_01 = malloc(cols*rows*3);
      int currPoint_01 = 0;
      jpeg_start_decompress(&cinfo_01);
      while(cinfo_01.output_scanline < cinfo_01.output_height) 
      {
         
         char *cutempPtr_01=&data_01[currPoint_01];
         jpeg_read_scanlines(&cinfo_01, (JSAMPARRAY)&cutempPtr_01, 1);
         currPoint_01+=cols*3;
      }
      jpeg_finish_decompress(&cinfo_01);
      jpeg_destroy_decompress(&cinfo_01);
      fclose(fp_01);

      fb_write_reverse(data_01, cols,rows);
      free(data_01);
      sleep(1);
     
      if((space== 1 && i == 7) || (space == 1 && i == 13) || (space == 2 && i == 10) || 
      (space == 2 && i == 18) || (space == 3 && i == 8) || (space == 3 && i == 15))
      {   loseflag_01 = 1;
     printf("You lose\n");
            break; // 패배 상황시 종료
     }
    }
   pthread_cancel(thread_gyro); //기울기 쓰레드 종료
   pthread_cancel(thread_buzzer);//배경음악 쓰레드 종료
   buzzerStopSong();
    sleep(1);
    if(loseflag_01 == 1){
      lcdtextwrite("     GAME OVER     ", "     GAME OVER     ", 3);
        gamelose(); // 종료 음악 재생
      return 0;
    }
    else{
      lcdtextwrite("     YOU WIN     ", "     YOU WIN     ", 3);
        gamewin(); // 승리 음악 재생
      return 1;
    }
   }
