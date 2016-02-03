#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUFFERSIZE 512

int main(){
	setbuf(stdout,NULL);
	int inp;
	scanf("%d", &inp);
	int ans=inp*inp;
	printf("%d", ans);
	return 0;
}