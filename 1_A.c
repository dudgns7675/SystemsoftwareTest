#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	char s[7] = "Hi,bro";

	while(1)
	{
		printf("(%p) %s \n", (void*)s, s);
		sleep(1);
	}

	return (EXIT_SUCCESS);
}
