#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "../libfbdev/libfbdev.h"
#include "../libjpeg/jpeglib.h"
#include <sys/msg.h>
#include "jpgAnimation.h"











    int screen_width;
    int screen_height;
    int bits_per_pixel;
    int line_length;
    int cols = 0, rows = 0;
	char *data_ani;
	
	
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
		//	cinfo.image_width,cinfo.image_height,cinfo.num_components, cinfo.output_scanline);
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
		//	cinfo.image_width,cinfo.image_height,cinfo.num_components, cinfo.output_scanline);
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

	int AnimationPrint_delay(char *jpg, int numberOfJpg){
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
		//	cinfo.image_width,cinfo.image_height,cinfo.num_components, cinfo.output_scanline);
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


	
	