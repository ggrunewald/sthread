#ifndef __list__
#define __list__

#include "sthread.h"

typedef struct threadList		//struct for list manipulation of threads
{
	struct tcb * first;		//pointer to the first element of list
	struct tcb * last;		//pointer to the last element of list
	int count;
}threadList;

//struct constructor
threadList * listInit();

//manipulation of thread lists
void insertThread(threadList* list, tcb* thread);
tcb* searchThread(int id);
int removeThread(threadList* list, int id);
int alreadyInList(threadList* list, int id);

#endif
