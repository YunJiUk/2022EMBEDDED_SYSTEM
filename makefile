final.elf : one.o two.o myProject.h
	gcc one.o two.o myProject.h -o four.elf

one.o : one.c myProject.h
	gcc one.c -c

two.o : two.c myProject.h
	gcc two.c -c

