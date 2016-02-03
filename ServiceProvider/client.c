#include <stdio.h>
#include <poll.h>
#include "semaphore.c"
#include <assert.h>
#include <fcntl.h>

#define maxs 105
#define key 256
#define BUFFERSIZE 512

int fifo_fd, sem_fifo, mutex;
struct pollfd fds[maxs];

struct Request{
	int pid;
	int servicenum;
}req;



int main(){
	char* fifo="sfifo";

	fifo_fd=open(fifo, 0666);
	sem_fifo=sem_init(key, 1, 0);
	mutex=sem_init(555, 1, 1);

	sem_wait(mutex, 0);

	req.pid=getpid();
	scanf("%d", &req.servicenum);		
	
	write(fifo_fd, &req, sizeof(struct Request));
	sem_wait(sem_fifo, 0);

	char rfifo[BUFFERSIZE], wfifo[BUFFERSIZE];
	sprintf(rfifo, "%d.in", req.pid);
	sprintf(wfifo, "%d.out", req.pid);

	int ifd, ofd;
	ofd=open(rfifo, 0666);
	ifd=open(wfifo, 0666);

	sem_signal(mutex, 0);

	setbuf(stdout,NULL);
	
	assert(ifd!=-1);
	assert(ofd!=-1);

	printf("Enter the number: ");
	int t;
	char inp[BUFFERSIZE];
	
	int size = read(0,inp,sizeof(inp));
	
	write(ofd, inp, size);
	read(ifd, inp, sizeof(inp));

	printf("The output is %s\n", inp);
	return 0;
}