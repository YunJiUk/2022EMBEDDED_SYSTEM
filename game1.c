#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include <sys/msg.h>
#include "gyro.h"
#include "../libfbdev/libfbdev.h"

#include "../libjpeg/jpeglib.h"
#include <dirent.h>
#include "buzzer.h"

pthread_t thread_gyro;
pthread_t thread_buzzer;

int C=1, D=3, E=5, F=6, G=8, A=10, B= 11; //B=12;
void *buzzerFunc()
{   while(1){
    buzzerInit();
    buz(A);    usleep(250000);
    buz2(C);     usleep(500000);
    buz2(D);    usleep(250000);
    buz2(C);    usleep(1000000);
    buz(A);    usleep(375000);
    buz2(C);    usleep(125000);    buzzerStopSong();   usleep(100);
    buz2(C);    usleep(250000);    buzzerStopSong();    usleep(100);
    buz2(C);    usleep(250000);    buz2(D);    usleep(250000);
    buz2(C);    usleep(750000);
    buz(A);    usleep(250000);
    buz2(C);    usleep(500000);    buzzerStopSong();    usleep(100);
    buz2(C);    usleep(250000);
    buz2(D);    usleep(250000);
    buz2(C);    usleep(250000);
    buz(A);    usleep(250000);
    buz(F);    usleep(250000);
    buz(G);    usleep(1500000);
    buz(B);    usleep(250000);    buzzerStopSong();    usleep(100);
    buz(B);    usleep(250000);    buzzerStopSong();    usleep(100);
    buz(B);    usleep(250000);
    buz(A);    usleep(250000);
    buz(G);    usleep(250000);
    buz(A);    usleep(250000);
    buz(B);    usleep(500000);    buzzerStopSong();    usleep(100);
    buz(B);    usleep(250000);    buzzerStopSong();    usleep(100);
    buz(B);    usleep(250000);    buzzerStopSong();    usleep(100);
    buz(B);    usleep(250000);
    buz(A);    usleep(250000);
    buz(G);    usleep(250000);
    buz(A);    usleep(250000);
    buz(B);    usleep(500000);
    buz(A);    usleep(250000);
    buz2(C);    usleep(500000);
    buz(B);    usleep(250000);
    buz(A);    usleep(250000);
    buz(G);    usleep(250000);
    buz(F);    usleep(250000);
    buz(E);    usleep(250000);
    buz(F);    usleep(1500000);
}   
    buzzerStopSong();
}
int space = 2;

void *gyroFunc(){
    Gyroinit();
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

int main (int argc, char **argv)
{
	pthread_create(&thread_gyro, NULL, gyroFunc, NULL);
    pthread_create(&thread_buzzer, NULL, buzzerFunc, NULL);

    int screen_width;
    int screen_height;
    int bits_per_pixel;
    int line_length;
    int cols = 0, rows = 0;
	char *data;
	
	//FrameBuffer init
    if ( fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0 )
	{
		printf ("FrameBuffer Init Failed\r\n");
		return 0;
	}
	//Clear FB.
	fb_clear();
	space = 2;
	int i=0;
	printf("%d", space);
	while (1){
	for (i=0; i<=19;i++)
	{
		int space2 = space;
		printf("%d", space);
		
		char filename[200]={0,};
		snprintf(filename,200,"./rundog/run_%d_%d.JPG",space2, i);
		printf("%d", space2);
		//FileRead
		int error=0;
		struct jpeg_decompress_struct cinfo;
		struct jpeg_error_mgr jerr;
	 	cinfo.err = jpeg_std_error(&jerr);
   		jpeg_create_decompress(&cinfo);
		FILE *fp = fopen(filename, "rb");
    	jpeg_stdio_src(&cinfo, fp);
		jpeg_read_header(&cinfo, TRUE); 
		//printf ("JPG %d by %d by %d, %d\n",
		//	cinfo.image_width,cinfo.image_height,cinfo.num_components, cinfo.output_scanline);
		cols = cinfo.image_width;
		rows = cinfo.image_height;

		data = malloc(cols*rows*3);
		int currPoint = 0;
		jpeg_start_decompress(&cinfo);
		while(cinfo.output_scanline < cinfo.output_height) 
		{
			//printf ("CInfoScanlines:%d\r\n",cinfo.output_scanline);
			char *tempPtr=&data[currPoint];
			jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&tempPtr, 1);
			currPoint+=cols*3;
		}
		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
		fclose(fp);

		fb_write_reverse(data, cols,rows);
		free(data);
		sleep(1);
		if((space2 == 1 && i == 7) || (space2 == 1 && i == 13) || (space2 == 2 && i == 10) || 
		(space2 == 2 && i == 18) || (space2 == 3 && i == 8) || (space2 == 3 && i == 15))
		{	printf("end.");
			break;
		}
	}
	sleep(4);
	}
	fb_close();
    return 0;
}
