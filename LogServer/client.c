#include <stdio.h>
#include "semaphore.c"

#define BUFFERSIZE 512
#define key 256

char* famous_fifo="lserver";
int famous_fifo_fd;

int ifd,ofd;
struct Request{
	int pid;
	char service[BUFFERSIZE];
}req;

int sem_fifo, mutex;
char rfifo[BUFFERSIZE], wfifo[BUFFERSIZE];

int main(){
	sem_fifo=sem_init(key, 1, 0);
	mutex=sem_init(555, 1, 1);

	famous_fifo_fd=open(famous_fifo, 0666);

	req.pid=getpid();
	scanf("%s", req.service);

	sem_wait(mutex, 0);

	write(famous_fifo_fd, &req, sizeof(struct Request));
	
	sem_wait(sem_fifo, 0);

	sprintf(rfifo, "%d.in", getpid());
	sprintf(wfifo, "%d.out", getpid());

	ifd=open(wfifo, 0666);
	ofd=open(rfifo, 0666);

	sem_signal(mutex, 0);

	printf("Enter the number: ");
	char buffer[BUFFERSIZE];
	scanf("%s", buffer);
	write(ofd, buffer, sizeof(buffer));

	read(ifd, buffer, sizeof(buffer));

	printf("The output is %s\n", buffer);
	return 0;
}