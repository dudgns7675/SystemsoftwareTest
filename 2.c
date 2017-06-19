#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int i;
	void *p;
	void (*f)();

	p = malloc(1024);

	for(i=0; i<1000; i++)
	{
		p = malloc(512);
	}

	printf("strace를 이용해 실행시키면 프로그램카운터가 변경되는 것을 확인할 수 있습니다. \n");
}
