#include "../include/sdata.h"
#include "../include/functions.h"
#include <stdio.h>

TCB* TCB_const(int id) 
{
	TCB* thread = malloc(sizeof(TCB));
	thread->tid = id;
	thread->next = 0;
	return thread;
}

void TCB_dest(TCB* thread) 
{
	free(thread);
}

smutex_t* mutex_const()
{
	smutex_t* mtx = malloc(sizeof(smutex_t));
	mtx->first = 0;
	mtx->last = 0;
	mtx->flag = 0;
	return mtx;
}

void mutex_dest(smutex_t* mtx) 
{
	free(mtx);
}

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

int main()
{
	TCB *thread1, *thread2, *thread3;

	thread1 = TCB_const(1);

	thread2 = TCB_const(2);

	thread3 = TCB_const(3);

	smutex_t * mutex;

	mutex = mutex_const();

	if(insert_in_blockqueue(mutex, thread1))
		printf("\nOk1");

	if(insert_in_blockqueue(mutex, thread2))
		printf("\nOk2");

	if(insert_in_blockqueue(mutex, thread3))
		printf("\nOk3");

	TCB * pesq = search_in_blockqueue(mutex, 3);

	printf("\n%d\n", pesq->tid); 

	return 0;
}
