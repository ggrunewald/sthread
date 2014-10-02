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
	////printf("insert thread %d\n", thread->tid);
	////printf("status da thread %d\n", thread->status);

	if(alreadyInList(list, thread->tid))
	{
		////printf("already in list!!!!\n");
		return;
	}

	if(list->first == NULL && list->last == NULL)
	{
		list->first = thread;
		list->last = thread;
	}
	else if(list->first == NULL && list->last != NULL)
	{
		////printf("ERRO 1 %d!\n", thread->tid);
	}
	else if(list->first != NULL && list->last == NULL)
	{
		////printf("ERRO 2 %d!\n", thread->tid);
	}
	else
	{
		list->last->next = thread;
		list->last = thread;
	}

	////printf("inserted %d!!!!\n", list->last->tid);

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
		tcb * tmp;
		tmp = list->first;
		list->first = list->last;
		list->count = 1;
		tmp->next = NULL;
		return SUCCESS;
	}
	else
	{
		tcb* pointer;
		tcb* previous;

		previous = list->first;
		pointer = list->first->next;

		if(list->first->tid == id)
		{
			tcb * tmp;
			tmp = list->first;
			list->first = list->first->next;
			list->count--;
			tmp->next = NULL;
			return SUCCESS;
		}

		while(pointer != NULL)
		{
			if(pointer->tid == id)
			{
				previous->next = pointer->next;
				pointer->next = NULL;
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
////printf("searching %d: ", id);
	while(temp != NULL)
	{
	////printf("%d ", temp->tid);
		if(temp->tid == id)
			return TRUE;

		else
			temp = temp->next;
	}
////printf("\n");
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
