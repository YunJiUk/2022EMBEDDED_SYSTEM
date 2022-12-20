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

pthread_t thread_buzzer;
int C=1, D1=3, D2 =4, E=5, F1=6, F2 = 7, G=8, A=10, B1= 11, B2=12;
void *buzzerFunc()
{   
   while(1){
    buzzerInit();
    buz(A);    usleep(250000);
    buz2(C);     usleep(500000);
    buz2(D1);    usleep(250000);
    buz2(C);    usleep(1000000);
    buz(A);    usleep(375000);
    buz2(C);    usleep(125000);    buzzerStopSong();   usleep(100);
    buz2(C);    usleep(250000);    buzzerStopSong();    usleep(100);
    buz2(C);    usleep(250000);    buz2(D1);    usleep(250000);
    buz2(C);    usleep(750000);
    buz(A);    usleep(250000);
    buz2(C);    usleep(500000);    buzzerStopSong();    usleep(100);
    buz2(C);    usleep(250000);
    buz2(D1);    usleep(250000);
    buz2(C);    usleep(250000);
    buz(A);    usleep(250000);
    buz(F1);    usleep(250000);
    buz(G);    usleep(1500000);
    buz(B1);    usleep(250000);    buzzerStopSong();    usleep(100);
    buz(B1);    usleep(250000);    buzzerStopSong();    usleep(100);
    buz(B1);    usleep(250000);
    buz(A);    usleep(250000);
    buz(G);    usleep(250000);
    buz(A);    usleep(250000);
    buz(B1);    usleep(500000);    buzzerStopSong();    usleep(100);
    buz(B1);    usleep(250000);    buzzerStopSong();    usleep(100);
    buz(B1);    usleep(250000);    buzzerStopSong();    usleep(100);
    buz(B1);    usleep(250000);
    buz(A);    usleep(250000);
    buz(G);    usleep(250000);
    buz(A);    usleep(250000);
    buz(B1);    usleep(500000);
    buz(A);    usleep(250000);
    buz2(C);    usleep(500000);
    buz(B1);    usleep(250000);
    buz(A);    usleep(250000);
    buz(G);    usleep(250000);
    buz(F1);    usleep(250000);
    buz(E);    usleep(250000);
    buz(F1);    usleep(1500000);
}   
    buzzerStopSong();
}

void gamewin(){
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

void gamelose(){
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

pthread_t thread_gyro;

int space =2;

void *gyroFunc(){
    Gyroinit();
    structgame1Msg Game1TxData;
    while(1){
    int x = Accelerometer();
    printf("%d", x);
    if(x > 4500){
        printf("down");
        space++;
        if(space>2){
         space = 3;
      }
    }
    else if(x < -1000){
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

int AnimationPrint_walk(){
   lcdtextwrite("RunRunRunRunRun", "Tilt to Avoid!", 3);
   loseflag_01 = 0;
   int i=0;
   pthread_create(&thread_gyro,NULL,gyroFunc,NULL);
   pthread_create(&thread_buzzer,NULL,buzzerFunc,NULL);
    for (i=0; i<=19;i++)
   {
     
   char filename_01[200]={0,};
      
   snprintf(filename_01,200,"./rundog/run_%d_%d.JPG",space, i);
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
            break;
     }
    }
   pthread_cancel(thread_gyro);
   pthread_cancel(thread_buzzer);
   buzzerStopSong();
    sleep(1);
    if(loseflag_01 == 1){
      lcdtextwrite("     GAME OVER     ", "     GAME OVER     ", 3);
        gamelose();
      return 0;
    }
    else{
      lcdtextwrite("     YOU WIN     ", "     YOU WIN     ", 3);
        gamewin();
      return 1;
    }
   }

   int AnimationPrint(char *jpg, int numberOfJpg){
   int k=0;
   for (k=0; k<=numberOfJpg;k++)
   {
      char filename_ani[200]={0,};
      snprintf(filename_ani,200,"%s-%04d.jpg",jpg,k);
      
      //FileRead
      int error_ani=0;
      struct jpeg_decompress_struct cinfo_ani;
      struct jpeg_error_mgr jerr_ani;
       cinfo_ani.err = jpeg_std_error(&jerr_ani);
         jpeg_create_decompress(&cinfo_ani);
      FILE *fp_ani = fopen(filename_ani, "rb");
       jpeg_stdio_src(&cinfo_ani, fp_ani);
      jpeg_read_header(&cinfo_ani, TRUE); 
      //printf ("JPG %d by %d by %d, %d\n",
      //   cinfo.image_width,cinfo.image_height,cinfo.num_components, cinfo.output_scanline);
      cols = cinfo_ani.image_width;
      rows = cinfo_ani.image_height;

      data_ani = malloc(cols*rows*3);
      int currPoint_ani = 0;
      jpeg_start_decompress(&cinfo_ani);
      while(cinfo_ani.output_scanline < cinfo_ani.output_height) 
      {
         //printf ("CInfoScanlines:%d\r\n",cinfo.output_scanline);
         char *tempPtr_ani=&data_ani[currPoint_ani];
         jpeg_read_scanlines(&cinfo_ani, (JSAMPARRAY)&tempPtr_ani, 1);
         currPoint_ani+=cols*3;
      }
      jpeg_finish_decompress(&cinfo_ani);
      jpeg_destroy_decompress(&cinfo_ani);
      fclose(fp_ani);

      fb_write_reverse(data_ani, cols,rows);
      free(data_ani);
      data_ani=NULL;
   }
   }

   int AnimationPrint_2(char *jpg, int numberOfJpg){
   int k=0;
   for (k=0; k<=numberOfJpg;k++)
   {
      char filename_ani[200]={0,};
      snprintf(filename_ani,200,"%s_%d.jpg",jpg,k);
      
      //FileRead
      int error_ani=0;
      struct jpeg_decompress_struct cinfo_ani;
      struct jpeg_error_mgr jerr_ani;
       cinfo_ani.err = jpeg_std_error(&jerr_ani);
         jpeg_create_decompress(&cinfo_ani);
      FILE *fp_ani = fopen(filename_ani, "rb");
       jpeg_stdio_src(&cinfo_ani, fp_ani);
      jpeg_read_header(&cinfo_ani, TRUE); 
      //printf ("JPG %d by %d by %d, %d\n",
      //   cinfo.image_width,cinfo.image_height,cinfo.num_components, cinfo.output_scanline);
      cols = cinfo_ani.image_width;
      rows = cinfo_ani.image_height;

      data_ani = malloc(cols*rows*3);
      int currPoint_ani = 0;
      jpeg_start_decompress(&cinfo_ani);
      while(cinfo_ani.output_scanline < cinfo_ani.output_height) 
      {
         //printf ("CInfoScanlines:%d\r\n",cinfo.output_scanline);
         char *tempPtr_ani=&data_ani[currPoint_ani];
         jpeg_read_scanlines(&cinfo_ani, (JSAMPARRAY)&tempPtr_ani, 1);
         currPoint_ani+=cols*3;
      }
      jpeg_finish_decompress(&cinfo_ani);
      jpeg_destroy_decompress(&cinfo_ani);
      fclose(fp_ani);

      fb_write_reverse(data_ani, cols,rows);
      free(data_ani);
      data_ani=NULL;
      }
   }

   int AnimationPrint_bath(char *jpg, int numberOfJpg){
   int k=0;
   for (k=0; k<=numberOfJpg;k++)
   {   

      structgame2Msg Game2TxData;
      int Game2msgQueue = msgget((key_t)GAME2MSG_ID, IPC_CREAT|0666);
      if(k==13||k==23||k==31||k==42){
         Game2TxData.messageType_jpg=7;
         Game2TxData.pressThisButton=1;
         msgsnd(Game2msgQueue, &Game2TxData, 4, 0);
      }
      if(k==21||k==30||k==38||k==44){
         Game2TxData.messageType_jpg=7;
         Game2TxData.pressThisButton=2;
         msgsnd(Game2msgQueue, &Game2TxData, 4, 0);
      }
      if(k==6||k==36||k==41){
         Game2TxData.messageType_jpg=7;
         Game2TxData.pressThisButton=3;
         msgsnd(Game2msgQueue, &Game2TxData, 4, 0);
      }
      if(k==29||k==37||k==43){
         Game2TxData.messageType_jpg=7;
         Game2TxData.pressThisButton=4;
         msgsnd(Game2msgQueue, &Game2TxData, 4, 0);
      }
      if(k==47){
         Game2TxData.messageType_jpg=7;
         Game2TxData.pressThisButton=111;
         msgsnd(Game2msgQueue, &Game2TxData, 4, 0);
      }
      
      


      char filename_ani[200]={0,};
      snprintf(filename_ani,200,"%s-%04d.jpg",jpg,k);
      
      //FileRead
      int error_ani=0;
      struct jpeg_decompress_struct cinfo_ani;
      struct jpeg_error_mgr jerr_ani;
       cinfo_ani.err = jpeg_std_error(&jerr_ani);
         jpeg_create_decompress(&cinfo_ani);
      FILE *fp_ani = fopen(filename_ani, "rb");
       jpeg_stdio_src(&cinfo_ani, fp_ani);
      jpeg_read_header(&cinfo_ani, TRUE); 
      //printf ("JPG %d by %d by %d, %d\n",
      //   cinfo.image_width,cinfo.image_height,cinfo.num_components, cinfo.output_scanline);
      cols = cinfo_ani.image_width;
      rows = cinfo_ani.image_height;

      data_ani = malloc(cols*rows*3);
      int currPoint_ani = 0;
      jpeg_start_decompress(&cinfo_ani);
      while(cinfo_ani.output_scanline < cinfo_ani.output_height) 
      {
         //printf ("CInfoScanlines:%d\r\n",cinfo.output_scanline);
         char *tempPtr_ani=&data_ani[currPoint_ani];
         jpeg_read_scanlines(&cinfo_ani, (JSAMPARRAY)&tempPtr_ani, 1);
         currPoint_ani+=cols*3;
      }
      jpeg_finish_decompress(&cinfo_ani);
      jpeg_destroy_decompress(&cinfo_ani);
      fclose(fp_ani);

      fb_write_reverse(data_ani, cols,rows);
      free(data_ani);
      data_ani=NULL;

      usleep(250000);
   }
   }


   int AnimationExit(){
   fb_close();
    return 0;}


   
   
