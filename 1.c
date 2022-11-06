#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char *argv[])
{
	int fd =0;
	int forwardBackward =0;
	int step =0;
	printf("Enter DIR:");
	scanf("%d", &forwardBackward);
	printf("Enter Skip#:");
	scanf("%d", &step);

	char buffer[100];
	fd=open(argv[1],O_RDWR);

	if(forwardBackward==1){
	lseek(fd,(off_t)0,SEEK_SET);
		{	
			while((read(fd, buffer, 1)) > 0){
				printf("%s",buffer);
				lseek(fd,(off_t)step-1,SEEK_CUR);
			}
		}
	}

	if(forwardBackward==-1){
	lseek(fd,(off_t)-1,SEEK_END);
		{
		while(1){
			read(fd, buffer, 1);
			printf("%s",buffer);
			if(lseek(fd,(off_t)-step-1,SEEK_CUR)==-1){
				break;
				}
			}
		}
	}
	close(fd); 
}