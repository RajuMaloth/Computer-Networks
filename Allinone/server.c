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

char buff[20];
char tot[1005];

void fun()
{
	printf("%s\n",tot);
	tot[0]='\0';
}

int main()
{
	int fd[2];
	int i;
	unlink("serverIn.txt");
	mkfifo("serverIn.txt",0666);
	struct pollfd serverPolls[10];
	int shmid=shmget(1234,100,0666 | IPC_CREAT);
	int *ids=(int*)shmat(shmid,NULL,0);
	ids[0]=getpid();

	signal(SIGUSR1,fun);

	serverPolls[0].fd=0;
	serverPolls[0].events=POLLIN;

	serverPolls[1].fd=open("serverIn.txt",O_RDWR);
	serverPolls[1].events=POLLIN;

	int pid=0;
	pipe(fd);
	pid=fork();

	if(pid==0){
		close(fd[0]);
		dup2(fd[1],1);
		execlp("./s1","./s1",NULL);
	}
	else{
		close(fd[1]);
		serverPolls[2].fd=fd[0];
		serverPolls[2].events=POLLIN;
	}
	tot[0]='\0';
	while(1){
		int ret=poll(serverPolls,3,500);
		if(ret>0){
			for(i=0;i<3;i++){
				if(serverPolls[i].revents & POLLIN){
					printf("%d\n",i);
					read(serverPolls[i].fd,buff,sizeof(buff));
				//	printf("%s\n", buff);
					strcat(tot,buff);
					serverPolls[i].revents=0;
				}
			}
		}
	}
	return 0;
}