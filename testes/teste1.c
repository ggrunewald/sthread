#include "../include/sthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5

int ids[NUM_THREADS];

void func_teste(void *arg)
{
	puts("Starting work");
	sleep(1);
	puts("Finishing work");
}

int main ()
{
	int i = 0;
	
	for (i = 0; i < NUM_THREADS; i++)
	{
		ids[i] = screate(0, &func_teste, NULL);
		printf("Thread %d created.\n", ids[i]);
		swait(ids[i]);
		printf("Thread %d finished.\n", ids[i]);
	}

	puts("Main finishing...");
	return 0;
}
