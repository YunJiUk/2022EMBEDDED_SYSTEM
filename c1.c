#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main  (int argc, char *argv[])
{
	int fdin, fdout;
    int filesize;
    ssize_t nread;
    if ( argc !=3 )
    {
        printf ("cp [source file] [dest file]\r\n");
        exit(-1);
    }

	fdin=open(argv[1],O_RDWR);
    fdout=open(argv[2],O_RDWR| O_CREAT | O_TRUNC,0644);
    filesize=lseek(fdin,(off_t)0,SEEK_END);
    lseek(fdin,(off_t)0,SEEK_SET);

    
    char *buffer = malloc(filesize);
    if (buffer == NULL)
    {   
        printf ("Memory Allocation Error, Buffer Failed\r\n");
        return -1;
    }

	while((nread = read(fdin, buffer, filesize)) > 0) 
    { 

        if(write(fdout, buffer, nread) < nread) 
        { 
            printf ("File copy failed\r\n");
            close(fdin); 
            close(fdout); 
        } 
    } 
	
	close(fdin); 
    close(fdout); 
    free(buffer);
}

