#include "../include/sthread.h"
#include "../include/list.h"
#include <stdlib.h>
#include <stdio.h>	//remover quando estiver tudo funcionando ok e não precisar mais debugar

int firstCall = TRUE;		//controls if must create the thread main
int threadCounter = 0;		//thread counter (usefull for identification)
int mutexInitiallized = FALSE;	//controls if user called smutex_init or slock should call it

tcb* tcb_const(int p)
{
	tcb * thread = (tcb*)malloc(sizeof(tcb));	//allocate memory for the thread
	thread->tid = threadCounter;			//initiallizes the thread with a defautl tid value
	threadCounter++;				//increment thread counter (only place where threadCounter should be manipulated)
	thread->next = NULL;				//set next to null
	thread->status = CREATED;
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

	mtx->blockList = listInit();

	mtx->locked = FALSE;

	return mtx;
}

void mutex_dest(smutex_t* mtx) 
{
	free(mtx);		//desallocate the memory
}

//creates a new thread
//returns its id if successful
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

	newThread->status = APT;

	//insert the newThread in the apt list of its priority
	insertThread(aptList[newThread->priority], newThread);

	printf("incluiu = %d em [%d]\n", aptList[newThread->priority]->last->tid, aptList[newThread->priority]->last->priority);

	return aptList[newThread->priority]->last->tid;
}

//current executing thread liberates the cpu
//and is inserted at the apt list
int syield()
{
	executingThread->status = APT;

	dispatcher(FALSE);

	return 0;
}


//the caller thread will wait until the tid teminate its execution
//return SUCCESS if successful
int swait(int tid)
{
	tcb * waitedThread = searchThread(tid);				//thread that is waited to finish its execution

	tcb * callerThread = executingThread;				//thread that called swait

	if(waitedThread == NULL)					//veririfies if user tried to wait for a non existant thread
		return ERROR;

	if(waitedThread->status == ENDED)
		return ERROR;

	callerThread->status = BLOCKED;					//blocks the caller thread

	getcontext(&returnContext); 					//terminated threads will return to this point

	int ret = 0;

	if(executingThread->tid != callerThread->tid)			//if its the waited thread (it already terminated its execution)
	{
		ret = 1;
printf("MINHA ROLA!");
		executingThread->status = ENDED;			//set its status to ended (will not run again)

		callerThread->status = APT;				//make the waiter APT again

		removeThread(blockedList, callerThread->tid);		//remove it from the blocked list

		insertThread(aptList[callerThread->priority], callerThread);

		dispatcher(FALSE);
	}
	
	else if(executingThread->tid == callerThread->tid)
	{
		while(TRUE)
		{
			//quits the eternal loop when the waited thread have terminated
			if(executingThread->tid == waitedThread->tid && ret == 1)
				break;

			dispatcher(FALSE);
		}
	}

	return SUCCESS;
}

int smutex_init(smutex_t *mtx)
{
	mtx = mutex_const();		//initiallizes the mutex

	mutexInitiallized = TRUE;

	if(mtx == NULL)
		return ERROR;

	return SUCCESS;
}

int slock (smutex_t *mtx)
{
/*
printf("SLOCK INICIO!\n");
	if(!mutexInitiallized)
		if(smutex_init(mtx) == ERROR)
			return ERROR;

	if(mtx->locked)
	{
		insertThread(mtx->blockList, executingThread);

		executingThread->status = BLOCKED;

		getcontext(&executingThread->context);

		while(mtx->locked)
			dispatcher(TRUE);
	}

	else
	{
		mtx->locked = TRUE;
	}
printf("SLOCK FIM!\n");
	return SUCCESS;
*/
}

int sunlock (smutex_t *mtx)
{
/*
printf("SUNLOCK INICIO!\n");
	if(!mtx->locked)
	{
		printf("SUNLOCK FIM ERROR!\n");
		return ERROR;
	}

	mtx->locked = FALSE;

printf("SUNLOCK 1!\n");

printf("que0\n");
mtx;
printf("que1\n");
mtx->blockList;
printf("que2\n");
mtx->blockList->count;
printf("que3\n");

/*
	if(mtx->blockList->count > 0)
	{
printf("SUNLOCK 2!\n");
		tcb * thread;

		thread = mtx->blockList->first;
printf("SUNLOCK 3!\n");
		removeThread(mtx->blockList, mtx->blockList->first->tid);
printf("SUNLOCK 4!\n");
		thread->status = APT;
printf("SUNLOCK 5!\n");
		insertThread(aptList[thread->priority], thread);
	}
*/
//printf("SUNLOCK FIM!\n");
	return SUCCESS;
}

int dispatcherInit()
{
	int i;

	//initializing the priority list with pointers to the lists of every possible priority
	for(i=0; i<3; i++)
		aptList[i] = listInit();

	//initializing the blocked threads list
	blockedList = listInit();

	//sets the firstCall to false, because this function should be executed once
	firstCall = FALSE;

	//creation of the first thread (main_thread)
	executingThread = tcb_const(2);			//MUDAR PARA PRIORIDADE BAIXA. UTILIZANDO PRIORIDADE
							//DIFERENTE DA ESTABELECIDA PARA FINS DE TESTES
	executingThread->status = EXECUTING;

	//gets the current context and save it in the newThread just created
	getcontext(&executingThread->context);

	//modify de newThread context to return to the context pointed by returnContext
	executingThread->context.uc_link = &returnContext;		
	executingThread->context.uc_stack.ss_sp = executingThread->stack;
	executingThread->context.uc_stack.ss_size = sizeof(executingThread->stack);

	if(executingThread->tid != 0)
		return ERROR;

	//verifies if something isn't intialized
	if(aptList == NULL	  ||
	   aptList[0] == NULL	  || 
	   aptList[1] == NULL	  || 
           aptList[2] == NULL	  ||
	   blockedList == NULL	  ||
	   executingThread == NULL)
		return ERROR;

	return SUCCESS;
}

void dispatcher(int isMutex)
{
	tcb * auxThread = executingThread;

	int i;

	for(i = 0; i < 3; i++)	
	{
		if(aptList[i]->count > 0)
		{
			executingThread = aptList[i]->first;				//gets next executed thread

			executingThread->status = EXECUTING;

			removeThread(aptList[i], executingThread->tid);

			if(auxThread->status == BLOCKED && !isMutex)
			{
				insertThread(blockedList, auxThread);			//insert in blocked list
				swapcontext(&auxThread->context, &executingThread->context);
			}
			else if(auxThread->status == BLOCKED && isMutex)
			{
				swapcontext(&auxThread->context, &executingThread->context);
			}
			else if(auxThread->status == APT)
			{
				insertThread(aptList[auxThread->priority], auxThread);	//insert in apt list
				swapcontext(&auxThread->context, &executingThread->context);
			}
			else if(auxThread->status == ENDED)
			{
				auxThread = NULL;
				setcontext(&executingThread->context);
			}
		}
	}
}

