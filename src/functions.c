#include "../include/sdata.h"
#include "../include/functions.h"

int insert(smutex_t *mtx, TCB * thread)
{
	mtx->last->next = thread;
	mtx->last = thread;

	return 0;
}

TCB * remove(smutex_t *mtx)
{
	TCB * temp;

	temp = mtx->first;

	mtx->first = mtx->first->next;

	return temp;
}

TCB * search(smutex_t *mtx, int id)
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
