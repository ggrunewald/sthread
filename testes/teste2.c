#include "sthread.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 1000
#define MAX_WORK 100000

mmutex_t mutex;
int workDone = 0;
int lastWork = -1;

void makeWork()
{
	syield();
	while(workDone < MAX_WORK)
	{
		slock(&mutex);
		lastWork = workDone;
		workDone++;
		sunlock(&mutex);
		if (workDone <= lastWork || workDone -1 != lastWork)
		{
			puts("Mutual Exclusion error!!!!!!!");
			printf("%d -> %d\n", lastWork, workDone);
			exit(0);
		}
		if (workDone < MAX_WORK)
		{
			printf("Work #%d done.\n", workDone);
			syield();
		}
	}
}

int main ()
{
	int ids[MAX_THREADS];
	int i = 0;

	puts ("Started.");

	for (i=0; i < MAX_THREADS; i++)
	{
		ids[i] = screate(0, &makeWork, NULL);
		printf("Thread using TID %d started working...\n", ids[i]);
	}

	for (i=0; i < MAX_THREADS; i++)
	{
		swait(ids[i]);
		printf("Thread using TID %d finished.\n", ids[i]);
	}

	return 0;
}
