#ifndef __list__
#define __list__

#include "sthread.h"

typedef struct threadList		//struct for list manipulation of threads
{
	struct tcb * first;		//data
	struct tcb * last;		//pointer to the next element of list
	int count;
}threadList;

//struct constructor
threadList * listInit();

//manipulation of thread lists
void insertThread(threadList* list, tcb* thread);
tcb* searchThread(int id);
void removeThread(threadList* list, int id);
int alreadyInList(threadList* list, int id);

#endif
