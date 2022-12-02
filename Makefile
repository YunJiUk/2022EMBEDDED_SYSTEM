all: colorledtest2

colorledtest2 : libMyPeri.a colorled.c colorled.h
	arm-linux-gnueabi-gcc colorled.c -l MyPeri -L. -o colorledtest2 -lpthread
libMyPeri.a : button.o led.o buzzer.o colorled.o
	arm-linux-gnueabi-ar rc libMyPeri.a led.o button.o buzzer.o colorled.o
colorled.o : colorled.h colorled.c
	arm-linux-gnueabi-gcc -c colorled.c -o colorled.o
buzzer.o : buzzer.h buzzer.c
	arm-linux-gnueabi-gcc -c buzzer.c -o buzzer.o
button.o : button.h button.c
	arm-linux-gnueabi-gcc -c button.c -lpthread -o button.o
led.o: led.h led.c
	arm-linux-gnueabi-gcc -c led.c -o led.o
rm : 
	rm *.o
	rm libMyPeri.a
