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
	if(alreadyInList(list, thread->tid) == TRUE)
		return;

	if(list->first == NULL)
	{
		list->first = thread;
		list->last = thread;
	}
	
	else
	{
		list->last->next = thread;
		list->last = thread;
	}

	list->count++;
}

//remove a thread from the list
void removeThread(threadList* list, int id)
{
	if(list->count == 0)	//if the list is empty, nothing to be removed
		return ;

	if(list->first->tid == id)
		list->first = list->first->next;

	else
	{
		tcb* pointer;
		tcb* previous;

		previous = list->first;
		pointer = list->first->next;

		while(pointer != NULL)
			if(pointer->tid == id)
			{
				previous->next = pointer->next;
				return ;
			}
			else
			{
				previous = pointer;
				pointer = pointer->next;
			}
	}

	list->count--;
}

int alreadyInList(threadList * list, int id)
{
	tcb * temp = list->first;	

	while(temp != 0)
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
		if(aptList[i]->count == 0)
			continue;

		temp = aptList[i]->first;

		while(temp != NULL)
		{
			if(temp->tid == id)
				return temp;

			else
				temp = temp->next;
		}
	}

	temp = blockedList->first;

	if(blockedList->count == 0)
		return 0;

	while(temp != 0)
	{
		if(temp->tid == id)
			return temp;

		else
			temp = temp->next;
	}
	
	return 0;
}
