all: buttontest2
buttontest2:libMyPeri.a mybuttontest.c 
	arm-linux-gnueabi-gcc mybuttontest.c -l MyPeri -L. -o buttontest2 -lpthread
libMyPeri.a: button.o led.o
	arm-linux-gnueabi-ar rc libMyPeri.a led.o button.o
button.o: button.h button.c
	arm-linux-gnueabi-gcc -c button.c -lpthread -o button.o
led.o: led.h led.c
	arm-linux-gnueabi-gcc -c led.c -o led.o
