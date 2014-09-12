#include "../include/sthread.h"

#include <stdlib.h>
#include <stdio.h>	//remover quando estiver tudo funcionando ok e não precisar mais debugar

int firstCall = TRUE;

TCB* tcb_const()
{
	TCB* thread = (TCB*)malloc(sizeof(TCB));
	thread->tid = 0;
	thread->next = NULL;
	return thread;
}

void tcb_dest(TCB* thread) 
{
	free(thread);
}

smutex_t* mutex_const()
{
	smutex_t* mtx = (smutex_t*)malloc(sizeof(smutex_t));
	mtx->first = NULL;
	mtx->last = NULL;
	mtx->flag = FALSE;
	return mtx;
}

void mutex_dest(smutex_t* mtx) 
{
	free(mtx);
}

int screate (int prio, void (*start)(void*), void *arg)
{
	if(firstCall)
		dispatcherInit();

	return 0;
}

int syield(void)
{

	return 0;
}

int swait(int tid)
{

	return 0;
}

int smutex_init(smutex_t *mtx)
{

	return 0;
}

int slock (smutex_t *mtx)
{

	return 0;
}

int sunlock (smutex_t *mtx)
{

	return 0;
}

void dispatcherInit(void)
{
	int i;

	for(i=0; i<3; i++)
	{
		aptList[i] = listInit();
	}

	blockedList = listInit();

	firstCall = FALSE;
}

