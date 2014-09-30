#include <stdlib.h>
#include <stdio.h>

#include "../include/list.h"

//list constructor
threadList * listInit()
{
	threadList * newList = (threadList *)malloc(sizeof(threadList));

	newList->first = NULL;
	newList->last = NULL;
	newList->count = 0;

	return newList;
}

//insert a thread at the list
void insertThread(threadList* list, tcb* thread)
{
	if(alreadyInList(list, thread->tid))
		return;

	if(list->first == NULL && list->last == NULL)
	{
		list->first = thread;
		list->last = thread;
	}
	else if(list->first == NULL && list->last != NULL)
	{
		printf("ERRO 1 %d!\n", thread->tid);
	}
	else if(list->first != NULL && list->last == NULL)
	{
		printf("ERRO 2 %d!\n", thread->tid);
	}
	else
	{
		list->last->next = thread;
		list->last = thread;
	}

	list->count++;
}

//remove a thread from the list
int removeThread(threadList* list, int id)
{
	if(list->count == 0)	//if the list is empty, nothing to be removed
		return ERROR;

	if(list->count == 1)
	{
		list->first = NULL;
		list->last = NULL;
		list->count = 0;
		return SUCCESS;
	}

	else if(list->count == 2)
	{
		list->first = list->last;
		list->count = 1;
		return SUCCESS;
	}
	else
	{
		tcb* pointer;
		tcb* previous;

		previous = list->first;
		pointer = list->first->next;

		while(pointer != NULL)
		{
			if(pointer->tid == id)
			{
				previous->next = pointer->next;
				list->count--;
				return SUCCESS;
			}
			else
			{
				previous = pointer;
				pointer = pointer->next;
			}
		}
	}

	return ERROR;
}

int alreadyInList(threadList * list, int id)
{
	tcb * temp = list->first;	

	while(temp != NULL)
	{
		if(temp->tid == id)
			return TRUE;

		else
			temp = temp->next;
	}

	return FALSE;
}

tcb* searchThread(int id)
{
	int i;

	tcb * temp;

	for(i = 0; i<3; i++)
	{
		if(aptList[i]->count <= 0)
			continue;

		temp = aptList[i]->first;

		while(temp != NULL)
			if(temp->tid == id)
				return temp;
			else
				temp = temp->next;
	}

	temp = blockedList->first;

	if(blockedList->count <= 0)
		return NULL;

	while(temp != 0)
		if(temp->tid == id)
			return temp;
		else
			temp = temp->next;

	return NULL;
}
