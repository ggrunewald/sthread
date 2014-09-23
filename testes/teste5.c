#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sthread.h"

void func(void* n)
{
	sleep(*(int*)n);
	syield();
	printf("%d\n", *(int*)n);
	return;
}

int main()
{
	int threads[5];
	int params[5]={2,4,3,1,5};
	int i;
	
	for(i=0;i<5;++i)
		threads[i] = screate(1, &func, (void *) &(params[i]));

	for(i=0;i<5;++i)
		swait(threads[i]);

	return 0;
}
