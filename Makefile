all: textlcdtest2

textlcdtest2 : libMyPeri.a textlcdtest.c lcdtext.h
	arm-linux-gnueabi-gcc textlcdtest.c -l MyPeri -L. -o textlcdtest2 -lpthread
libMyPeri.a : button.o led.o buzzer.o fnd.o lcdtext.o
	arm-linux-gnueabi-ar rc libMyPeri.a led.o button.o buzzer.o fnd.o lcdtext.o
lcdtext.o : lcdtext.h lcdtext.c
	arm-linux-gnueabi-gcc -c lcdtext.c -o lcdtext.o
fnd.o : fnd.h fnd.c
	arm-linux-gnueabi-gcc -c fnd.c -o fnd.o
buzzer.o : buzzer.h buzzer.c
	arm-linux-gnueabi-gcc -c buzzer.c -o buzzer.o
button.o : button.h button.c
	arm-linux-gnueabi-gcc -c button.c -lpthread -o button.o
led.o: led.h led.c
	arm-linux-gnueabi-gcc -c led.c -o led.o
rm : 
	rm *.o
	rm libMyPeri.a
