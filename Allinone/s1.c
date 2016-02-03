#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/msg.h>
#include<string.h>
#include<sys/ipc.h>
#include<unistd.h>
#include<signal.h>
#include<poll.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<pthread.h>
#include<poll.h>
#include<sys/shm.h>

int main()
{
	char str[20];

	printf("Hello\n");
	while(1){
		scanf("%s",str);
		write(1,str,sizeof(str));
	}
	return 0;
}