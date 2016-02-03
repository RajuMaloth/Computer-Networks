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
	int fl;
	int shmid=shmget(1234,100,0666);
	int *ids=(int*)shmat(shmid,NULL,0);
	while(1){
		scanf("%d",&fl);
		printf("%d %d\n", fl,ids[0]);
		if(fl==1){
			kill(ids[0],SIGUSR1);
		}
	}
}