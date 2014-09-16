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
threadList* insertThread(threadList* list, TCB* thread)
{
	if(thread == NULL)
		return NULL;

	//if list isn't initialized yet, initialize it
	if(list == NULL)
	{
		list = listInit();
		list->thread = thread;
		return list;
	}

	//if first node included
	if(list->thread == NULL)
	{
		list->thread = thread;
		return list;
	}

	struct threadList* auxList = list;

	//finds the last node of the queue
	while(auxList->next != NULL)
		auxList = auxList->next;

	auxList->next = listInit();
	auxList = auxList->next;
	auxList->thread = thread;
	auxList->next = NULL;

	return list;
}


//manipulation of blocked threads by mutex
int insertBlockqueue(smutex_t *mtx, TCB * thread)
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

TCB * removeBlockqueue(smutex_t * mtx)
{
	TCB * temp;

	temp = mtx->first;

	mtx->first = mtx->first->next;

	return temp;
}

TCB * searchBlockqueue(smutex_t * mtx, int id)
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
