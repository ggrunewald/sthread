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
threadList * insertThread(threadList* list, tcb* thread)
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

	threadList* auxList = list;

	//finds the last node of the queue
	while(auxList->next != NULL)
		auxList = auxList->next;

	auxList->next = listInit();
	auxList = auxList->next;
	auxList->thread = thread;
	auxList->next = NULL;

	return list;
}

tcb * threadSearch(threadList* list, int id)
{
	if(list == NULL)
		return NULL;

	threadList * auxList;

	auxList = list;

	while(auxList != NULL)
		if(auxList->thread->tid == id)
			return auxList->thread;
		else
			auxList = auxList->next;
}

threadList * removeThread(threadList* list, tcb * thread)
{
	//if not initialized list, no list to manipulate
	if(list == NULL)
		return NULL;

	//if empty list, nothing to remove
	if(list->thread == NULL)
		return list;

	//FIFO, always removes the first element of the queue
	if(list->thread->tid == thread->tid)
		return list->next;

	threadList * auxList, * prevList;

	prevList = list;
	auxList = list->next;

	while(auxList != NULL)
	{
		if(auxList->thread->tid == thread->tid)
		{
			prevList->next = auxList->next;

			return list;
		}
		else
		{
			prevList = auxList;

			auxList = auxList->next;
		}
	}

	return NULL;
}

//manipulation of blocked threads by mutex
int insertBlockqueue(smutex_t *mtx, tcb * thread)
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

tcb * removeBlockqueue(smutex_t * mtx)
{
	tcb * temp;

	temp = mtx->first;

	mtx->first = mtx->first->next;

	return temp;
}

tcb * searchBlockqueue(smutex_t * mtx, int id)
{
	tcb * temp;

	temp = mtx->first;

	while(temp != 0)
		if(temp->tid == id)
			return temp;
		else
			temp = temp->next;
	
	return 0;
}
