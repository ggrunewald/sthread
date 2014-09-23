#include <stdio.h>
#include <stdlib.h>
#include "sthread.h"

#define MAX_THREADS 50

void functionWithParam (void* parameter)
{
	printf("Thread executed funcion with parameter \"%d\"\n", *(int*)parameter);
}

int main ()
{
	int threads[MAX_THREADS];
	int i;

	for (i = 0; i < MAX_THREADS; i++)
	{
		threads[i] = screate(2, &functionWithParam, (void*)&i);
		swait(threads[i]);
	}
	
	return 0;
}
