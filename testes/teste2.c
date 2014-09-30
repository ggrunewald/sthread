#include "../include/sthread.h"
#include "../include/list.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 2
#define MAX_WORK 15

smutex_t mutex;
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
			printf("Work #%d done by thread #%d. In aptList: %d\n", workDone, executingThread->tid, aptList[executingThread->priority]->first->tid);
			printf("Still %d threads in aptList[%d].\n\n", aptList[executingThread->priority]->count, executingThread->priority);
			syield();
		}
		printf("#%d ended.\n", executingThread->tid);
		printf("In aptList: %d\n", aptList[executingThread->priority]->first->tid);
	}
printf("workDone: %d.\n", workDone);
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
