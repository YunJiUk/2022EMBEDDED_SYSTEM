all: buzzertest2

buzzertest2 : libMyPeri.a buzzertest.c buzzer.h
	arm-linux-gnueabi-gcc buzzertest.c -l MyPeri -L. -o buzzertest2 -lpthread
libMyPeri.a : button.o led.o buzzer.o
	arm-linux-gnueabi-ar rc libMyPeri.a led.o button.o buzzer.o
buzzer.o : buzzer.h buzzer.c
	arm-linux-gnueabi-gcc -c buzzer.c -o buzzer.o
button.o : button.h button.c
	arm-linux-gnueabi-gcc -c button.c -lpthread -o button.o
led.o: led.h led.c
	arm-linux-gnueabi-gcc -c led.c -o led.o
rm : 
	rm *.o
	rm libMyPeri.a
