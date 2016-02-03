#include <stdio.h>
#include <poll.h>
#include "semaphore.c"

#define maxs 105
#define BUFFERSIZE 512
#define key 256

int sem_fifo, fifo_fd;
struct pollfd fds[maxs];
int clients;

struct Request{
	int pid;
}req;

struct Message{
	int pid;
	char message[BUFFERSIZE];
}msg;

int wfd[maxs];

int main(){

	char* fifo="sfifo";

	sem_fifo=sem_init(key, 1, 0);

	unlink(fifo);
	mkfifo(fifo, 0666);
	fifo_fd=open(fifo, 0666);
	fds[0].fd=fifo_fd;
	fds[0].events=POLLIN;
	fds[0].revents=0;
	clients++;

	//polling

	while(1){
		int status=poll(fds, clients, 0);

		if(status>0){
			if(fds[0].revents & POLLIN){
				read(fds[0].fd, &req, sizeof(struct Request));

				printf("%d Requested\n", req.pid);

				char rfifo[BUFFERSIZE], wfifo[BUFFERSIZE];
				sprintf(rfifo, "%d.in", req.pid);
				sprintf(wfifo, "%d.out", req.pid);

				unlink(rfifo);
				unlink(wfifo);
				mkfifo(rfifo, 0666);
				mkfifo(wfifo, 0666);

				wfd[clients]=open(wfifo, 0666);
				fds[clients].fd=open(rfifo, 0666);
				fds[clients].events=POLLIN;
				fds[clients].revents=0;

				clients++;
				printf("%d connection granted\n", req.pid);
				sem_signal(sem_fifo, 0);

			}
			int i;
			for(i=1; i<clients; i++){
				if(fds[i].revents==POLLIN){
					read(fds[i], &msg, sizeof(struct Message));
					int j;
					for(j=1; j<clients; j++){
						if(j!=i){
							write(wfd[j], &msg, sizeof(struct Message));
						}
					}
				}
			}
		}
	}

	return 0;
}