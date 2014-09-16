#include "../include/sthread.h"
#include "../include/list.h"
#include <stdlib.h>
#include <stdio.h>	//remover quando estiver tudo funcionando ok e não precisar mais debugar

int firstCall = TRUE;	//true if is the first time that screate is called
int threadCounter = 0;	//thread counter (usefull for identification)

tcb* tcb_const(int p)
{
	tcb * thread = (tcb*)malloc(sizeof(tcb));	//allocate memory for the thread
	thread->tid = threadCounter;			//initiallizes the thread with a defautl tid value
	threadCounter++;				//increment thread counter (only place where threadCounter should be manipulated)
	thread->next = NULL;				//set next to null
	thread->status = APT;
	thread->priority = p;

	if(thread->priority < 0 || thread->priority > 2)
		return NULL;

	return thread;
}

void tcb_dest(tcb* thread) 
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

	tcb * newThread;

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

	//insert the newThread in the apt list of its priority
	aptList[newThread->priority] = insertThread(aptList[newThread->priority], newThread);

	//insert the newThread in the list that holds all managed threads
	allThreadsList = insertThread(allThreadsList, newThread);

	return newThread->tid;
}

int syield()
{

	return 0;
}

int swait(int tid)
{
	tcb * thread = threadSearch(allThreadsList, tid);

	if(thread == NULL)		//veririfies if user tried to wait for a non existant thread
		return ERROR;

	executingThread->status = BLOCKED;

	blockedList = insertThread(blockedList, executingThread);

	int ret = 0;

	getcontext(&returnContext); 	//terminated threads will return to this point.

	if(executingThread->tid != 0)
	{
		if(executingThread->tid == tid)
		{
			ret = 1;

			blockedList = removeThread(blockedList, thread);

			aptList[thread->priority] = insertThread(aptList[thread->priority], thread);

			dispatcher();
		}
	}
	
	while(TRUE)
	{			
		//quits the eternal loop when the waited thread have terminated
		if(executingThread->tid == thread->tid && ret == 1) 
			break;				
		
		dispatcher();							
	}

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

	//initializing the list that holds all the threads that have been created
	allThreadsList = listInit();

	//sets the firstCall to false, because this function should be executed once
	firstCall = FALSE;

	//creation of the first thread (main_thread)
	executingThread = tcb_const(2);

	//insert the newThread in the list that holds all managed threads
	allThreadsList = insertThread(allThreadsList, executingThread);

	if(executingThread->tid != 0)
		return ERROR;

	//verifies if something isn't intialized
	if(aptList == NULL	  ||
	   aptList[0] == NULL	  || 
	   aptList[1] == NULL	  || 
           aptList[2] == NULL	  ||
	   blockedList == NULL	  ||
	   allThreadsList == NULL ||
	   executingThread == NULL)
		return ERROR;

	return SUCCESS;
}

void dispatcher()
{
	tcb * auxThread = executingThread;
	
	if(aptList[0]->thread != NULL)
	{
		executingThread = aptList[0]->thread;

		executingThread->status = EXECUTING;

		aptList[0] = removeThread(aptList[0], aptList[0]->thread);

		swapcontext(&auxThread->context, &executingThread->context);
	}

	else if(aptList[1]->thread != NULL)
	{
		executingThread = aptList[1]->thread;

		executingThread->status = EXECUTING;

		aptList[1] = removeThread(aptList[1], aptList[1]->thread);

		swapcontext(&auxThread->context, &executingThread->context);
	}

	else if(aptList[2]->thread != NULL)
	{
		executingThread = aptList[2]->thread;

		executingThread->status = EXECUTING;

		aptList[2] = removeThread(aptList[2], aptList[2]->thread);

		swapcontext(&auxThread->context, &executingThread->context);
	}
}

