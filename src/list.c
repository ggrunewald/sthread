#include <stdlib.h>
#include <stdio.h>

#include "../include/list.h"

//list constructor
threadList * listInit()
{
	threadList * newList = (threadList *)malloc(sizeof(threadList));
	newList->next = NULL;
	newList->thread = NULL;

	return newList;
}

//manipulation of blocked threads by dispatcher and priority apt list
struct threadList* includeThread(struct threadList* list, struct TCB* tcb)
{

}


//manipulation of blocked threads by mutex
int insert_in_blockqueue(smutex_t *mtx, TCB * thread)
{
	if(mtx->first == 0)
	{
		mtx->first = thread;
		mtx->last = thread;
	}
	
	else
	{
		mtx->last->next = thread;
		mtx->last = thread;
	}

	return 1;
}

TCB * remove_from_blockqueue(smutex_t * mtx)
{
	TCB * temp;

	temp = mtx->first;

	mtx->first = mtx->first->next;

	return temp;
}

TCB * search_in_blockqueue(smutex_t * mtx, int id)
{
	TCB * temp;

	temp = mtx->first;

	while(temp != 0)
		if(temp->tid == id)
			return temp;
		else
			temp = temp->next;
	
	return 0;
}
