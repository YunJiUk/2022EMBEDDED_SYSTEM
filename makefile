final.elf : 1.o 2.o myProject.h main.o
	gcc 1.o 2.o myProject.h main.o -o final.elf

main.o : main.c myproject.h
	gcc main.c -c

1.o : 1.c myProject.h
	gcc 1.c -c

2.o : 2.c myProject.h
	gcc 2.c -c

