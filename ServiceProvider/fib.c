#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUFFERSIZE 512

int process(int inp){
	if(inp<=2)return inp-1;
	int ans=0;
	int a=0;
	int b=1;
	int i;
	for(i=2; i<=inp; i++){
		ans=a+b;
		a=b;
		b=ans;
	}
	return ans;
}

int main(){
	setbuf(stdout,NULL);
	int inp;
	scanf("%d", &inp);
	int ans=process(inp);
	printf("%d", ans);
	return 0;
}