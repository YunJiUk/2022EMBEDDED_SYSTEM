
all: ledtest2

ledtest2 : libMyPeri.a ledtest.c led.h
	arm-linux-gnueabi-gcc ledtest.c -l MyPeri -L. -o ledtest2
libMyPeri.a : led.o 
	arm-linux-gnueabi-ar rc libMyPeri.a led.o 
led.o: led.h led.c
	arm-linux-gnueabi-gcc -c led.c -o led.o
