#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include "semaphore.c"
#include <assert.h>
#include <fcntl.h>

#define maxs 105
#define key 256
#define BUFFERSIZE 512

int fifo_fd, sem_fifo;
struct pollfd fds[maxs];

struct Request{
	int pid;
	int servicenum;
}req;

char* services[5]={"", "fib", "square"};

int main(){

	char* fifo="sfifo";

	unlink(fifo);
	mkfifo(fifo, 0666);
	fifo_fd=open(fifo, 0666);
	sem_fifo=sem_init(key, 1, 0);
	fds[0].fd=fifo_fd;
	fds[0].events=POLLIN;
	fds[0].revents=0;

	
	
	while(1){
		int status=poll(fds, 1, 0);

		if(status>0 && (fds[0].revents == POLLIN)){

			read(fds[0].fd, &req, sizeof(struct Request));
			
			
			char rfifo[BUFFERSIZE], wfifo[BUFFERSIZE];
			sprintf(rfifo, "%d.in", req.pid);
			sprintf(wfifo, "%d.out", req.pid);

			unlink(rfifo);
			unlink(wfifo);
			mkfifo(rfifo, 0666);
			mkfifo(wfifo, 0666);

			int ifd, ofd;
			ifd=open(rfifo, 0666);
			ofd=open(wfifo, 0666);
			
			sem_signal(sem_fifo, 0);


			int p=fork();
			if(p==0){

				dup2(ifd,0);
				dup2(ofd,1);

				char path[BUFFERSIZE];
				char cwd[BUFFERSIZE];
				getcwd(cwd, 105);

				//the error is here debug it!!
				sprintf(path, "%s/%s", cwd, services[req.servicenum]);
				char *dummy;
			
				execl(path, dummy, NULL);
			}
			else wait(&status);
		}
		
	}
	return 0;
}