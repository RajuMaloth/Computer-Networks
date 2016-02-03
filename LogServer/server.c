#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include "semaphore.c"

#define BUFFERSIZE 512
#define maxs 105
#define key 256

int ifd, key_fd;

char* famous_fifo="lserver";
int famous_fifo_fd;

struct pollfd fds[maxs];

struct Request{
	int pid;
	char service[BUFFERSIZE];
}req;

char rfifo[BUFFERSIZE], wfifo[BUFFERSIZE];
int ifd,ofd;
int sem_fifo;


int main(){
	printf("Log server started\n");

	sem_fifo=sem_init(key, 1, 0);

	unlink(famous_fifo);
	mkfifo(famous_fifo, 0666);
	famous_fifo_fd=open(famous_fifo, 0666);
	fds[0].fd=famous_fifo_fd;
	fds[0].events=POLLIN;
	fds[0].revents=0;

	while(1){
		int status=poll(fds, 1, 0);

		if(status>0 && (fds[0].revents==POLLIN)){
			read(fds[0].fd, &req, sizeof(struct Request));

			printf("Client %d requested %s service\n", req.pid, req.service);

			sprintf(rfifo, "%d.in", req.pid);
			sprintf(wfifo, "%d.out", req.pid);

			unlink(rfifo);
			unlink(wfifo);
			mkfifo(rfifo, 0666);
			mkfifo(wfifo, 0666);

			ifd=open(rfifo, 0666);
			ofd=open(wfifo, 0666);

			sem_signal(sem_fifo, 0);

			dup2(ifd, 0);

			char command[BUFFERSIZE];
			sprintf(command, "./%s", req.service);
			FILE* fd=popen(command,"r");


			char buffer[BUFFERSIZE];
			fgets(buffer, sizeof(buffer), fd);

			write(ofd, buffer, sizeof(buffer));

			printf("Output sent to Client %d\n", req.pid);
		}
	}
	return 0;
}
