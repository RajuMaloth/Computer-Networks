#include <stdio.h>
#include <pthread.h>
#include "semaphore.c"

#define maxs 105
#define BUFFERSIZE 512
#define key 256

int sem_fifo, fifo_fd, mutex;
int clients;

struct Request{
	int pid;
}req;

struct Message{
	int pid;
	char message[BUFFERSIZE];
}msg;

int ifd,ofd;

void* read_chat(void* arg){
	struct Message mssg;
	while(1){
		read(ifd, &mssg, sizeof(struct Message));
		printf("Client %d: %s\n", mssg.pid, mssg.message);
	}
}

int main(){

	char* fifo="sfifo";

	sem_fifo=sem_init(key, 1, 0);
	mutex=sem_init(555, 1, 1);

	fifo_fd=open(fifo, 0666);
	
	sem_wait(mutex, 0);
	req.pid=getpid();
	write(fifo_fd, &req, sizeof(struct Request));
	sem_wait(sem_fifo, 0);

	char rfifo[BUFFERSIZE], wfifo[BUFFERSIZE];
	sprintf(rfifo, "%d.in", req.pid);
	sprintf(wfifo, "%d.out", req.pid);

	ifd=open(wfifo, 0666);
	ofd=open(rfifo, 0666);
	sem_signal(mutex, 0);

	pthread_t rthread;
	pthread_create(&rthread, NULL, read_chat, NULL);

	while(1){
		msg.pid=getpid();
		read(0, msg.message, sizeof(msg.message));
		write(ofd, &msg, sizeof(struct Message));
	}
	return 0;
}