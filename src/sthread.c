#include "../include/sthread.h"
#include <stdlib.h>
#include <stdio.h>	//remover quando estiver tudo funcionando ok e não precisar mais debugar

int firstCall = TRUE;	//true if is the first time that screate is called
int threadCounter = 0;	//thread counter (usefull for identification)

TCB* tcb_const(int p)
{
	TCB * thread = (TCB*)malloc(sizeof(TCB));	//allocate memory for the thread
	thread->tid = threadCounter;			//initiallizes the thread with a defautl tid value
	threadCounter++;				//increment thread counter (only place where threadCounter should be manipulated)
	thread->next = NULL;				//set next to null
	thread->status = APT;
	thread->priority = p;

	if(thread->priority < 0 || thread->priority > 2)
		return NULL;

	return thread;
}

void tcb_dest(TCB* thread) 
{
	free(thread);		//desallocate the memory
}

smutex_t* mutex_const()
{
	smutex_t* mtx = (smutex_t*)malloc(sizeof(smutex_t));	//allocate memory for the mutex
	mtx->first = NULL;					//set first and last to null, the block queue
	mtx->last = NULL;					//is empty in the initialization of mutex
	mtx->flag = FALSE;
	return mtx;
}

void mutex_dest(smutex_t* mtx) 
{
	free(mtx);		//desallocate the memory
}

int screate (int prio, void (*start)(void*), void *arg)
{
	if(firstCall)
		if (dispatcherInit() == ERROR)	//calls dispatcher initialization (also creates thread_main)
			return ERROR;		//and verify if error at the initialization

	TCB * newThread;

	newThread = tcb_const(prio);		//create the new child thread 

	if(newThread == NULL)
		return ERROR;

	getcontext(&newThread->context);	//gets the current context and save it in the newThread just created

	//modify de newThread context to return to the context pointed by returnContext
	newThread->context.uc_link = &returnContext;		
	newThread->context.uc_stack.ss_sp = newThread->stack;
	newThread->context.uc_stack.ss_size = sizeof(newThread->stack);

	//modify de newThread context to execute the start function
	makecontext(&newThread->context, (void*)(*start), 1, arg);

	//include the newThread in the apt list of its priority
	aptList[newThread->priority] = insertThread(aptList[newThread->priority], newThread);

	//printf("%d\n", aptList[newThread->priority]->thread->priority);
	newThread->priority;
	aptList[newThread->priority];
	aptList[newThread->priority]->thread;	//ERRO AQUI	

	return SUCCESS;
}

int syield()
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

int dispatcherInit()
{
	int i;

	//initializing the priority list with pointers to the lists of every possible priority
	for(i=0; i<3; i++)
	{
		aptList[i] = listInit();
	}

	//initializing the blocked threads list
	blockedList = listInit();

	//sets the firstCall to false, because this function should be executed once
	firstCall = FALSE;

	//creation of the first thread (main_thread)
	executingThread = tcb_const(2);

	if(executingThread->tid != 0)
		return ERROR;

	if(aptList == NULL	||
	   aptList[0] == NULL	|| 
	   aptList[1] == NULL	|| 
           aptList[2] == NULL	||
	   blockedList == NULL	||
	   executingThread == NULL)
		return ERROR;

	return SUCCESS;
}

