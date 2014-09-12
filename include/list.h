#ifndef __list__
#define __list__

#include "sthread.h"

typedef struct threadList		//struct for list manipulation of threads
{
	TCB * thread;			//data
	struct threadList * next;	//pointer to the next element of list
}threadList;

//struct constructor
threadList * listInit();

//manipulation of blocked threads by dispatcher and priority apt list
struct threadList* includeThread(struct threadList* list, struct TCB* tcb);

//manipulation of blocked threads by mutex
int insert_in_blockqueue(smutex_t *mtx, TCB *thread);
TCB * remove_from_blockqueue(smutex_t *mtx);
TCB * search_in_blockqueue(smutex_t *mtx, int id);

#endif
