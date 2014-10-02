#include "../include/sthread.h"
#include "../include/list.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 30
#define MAX_WORK 1500

smutex_t mutex;
int workDone = 0;
int lastWork = -1;

//int paizinho = 0;

void makeWork()
{
	//printf("makeWork 1, workDone = %d\n", workDone);
	syield();

	while(workDone < MAX_WORK)
	{
		//printf("contador: %d\n\n\n\n", paizinho);
//		paizinho++;
		//printf("makeWork 2, workDone = %d\n", workDone);
		//slock(&mutex);
		lastWork = workDone;
		workDone++;
		//sunlock(&mutex);
		//printf("makeWork 3, workDone = %d\n", workDone);
		if (workDone <= lastWork || workDone -1 != lastWork)
		{
			//puts("Mutual Exclusion error!!!!!!!");
			//printf("%d -> %d\n", lastWork, workDone);
			exit(0);
		}
		if (workDone < MAX_WORK)
		{

			//printf("Work #%d done by thread #%d. In aptList: %d\n", workDone, executingThread->tid, aptList[executingThread->priority]->first->tid);
			//puts("makeWork 4");
			syield();
		}

		////printf("#%d ended.\n", executingThread->tid);
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
		//printf("Thread using TID %d started working...\n", ids[i]);
	}

	//puts ("Todas as threads criadas.");

	for (i=0; i < MAX_THREADS; i++)
	{
		//puts ("Antes do swait");
		swait(ids[i]);
		//puts ("Depois do swait");
		printf("Thread using TID %d finished.\n", ids[i]);
	}

	return 0;
}
