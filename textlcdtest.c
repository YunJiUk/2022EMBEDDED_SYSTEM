#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "lcdtext.h"

int main(void){

    lcdtextwrite("1st line write", "Don't write", 1);  //11
    sleep(1);
    lcdtextwrite("Don't write", "2nd line write", 2);  //44
    sleep(1);
    lcdtextwrite("Both write 1", "Both write 2", 3);  //5566
}

