#ifndef __list__
#define __list__

#include "sthread.h"

typedef struct threadList		//struct for list manipulation of threads
{
	struct tcb * thread;			//data
	struct threadList * next;	//pointer to the next element of list
}threadList;

//struct constructor
threadList * listInit();

//manipulation of blocked threads by dispatcher and priority apt list
threadList* insertThread(threadList* list, tcb* thread);

//manipulation of blocked threads by mutex
int insertBlockqueue(smutex_t *mtx, tcb *thread);
tcb * removeBlockqueue(smutex_t *mtx);
tcb * searchBlockqueue(smutex_t *mtx, int id);

#endif
