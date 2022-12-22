all: libMyPeri.a

libMyPeri.a : button.o led.o buzzer.o colorled.o fnd.o temperature.o lcdtext.o jpgAnimation.o jpgViewer.o gyro.o
	arm-linux-gnueabi-ar rc libMyPeri.a led.o button.o buzzer.o colorled.o fnd.o temperature.o lcdtext.o jpgAnimation.o jpgViewer.o gyro.o
gyro.o: gyro.h gyro.c
	arm-linux-gnueabi-gcc -c gyro.c -o gyro.o
jpgAnimation.o: jpgAnimation.h jpgAnimation.c
	arm-linux-gnueabi-gcc -c jpgAnimation.c -o jpgAnimation.o
jpgViewer.o: jpgViewer.h jpgViewer.c
	arm-linux-gnueabi-gcc -c jpgViewer.c -o jpgViewer.o
touch.o: touch.h touch.c
	arm-linux-gnueabi-gcc -c touch.c -o touch.o
temperature.o: temperature.h temperature.c
	arm-linux-gnueabi-gcc -c temperature.c -o temperature.o
lcdtext.o: lcdtext.h lcdtext.c
	arm-linux-gnueabi-gcc -c lcdtext.c -o lcdtext.o
fnd.o: fnd.h fnd.c
	arm-linux-gnueabi-gcc -c fnd.c -o fnd.o
colorled.o : colorled.h colorled.c
	arm-linux-gnueabi-gcc -c colorled.c -o colorled.o
buzzer.o : buzzer.h buzzer.c
	arm-linux-gnueabi-gcc -c buzzer.c -o buzzer.o
button.o : button.h button.c
	arm-linux-gnueabi-gcc -c button.c -lpthread -o button.o
led.o: led.h led.c
	arm-linux-gnueabi-gcc -c led.c -o led.o
rm : 
	
	rm libMyPeri.a