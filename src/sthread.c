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

	mtx->flag = FALSE;

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

int syield()
{

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

		executingThread->status = ENDED;			//set its status to ended (will not run again)

		callerThread->status = APT;				//make the waiter APT again

		removeThread(blockedList, callerThread->tid);		//remove it from the blocked list

		insertThread(aptList[callerThread->priority], callerThread);

		dispatcher();
	}
	
	else if(executingThread->tid == callerThread->tid)
	{
		while(TRUE)
		{
			//quits the eternal loop when the waited thread have terminated
			if(executingThread->tid == waitedThread->tid && ret == 1)
				break;

			dispatcher();
		}
	}

	return SUCCESS;
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
		aptList[i] = listInit();

	//initializing the blocked threads list
	blockedList = listInit();

	//sets the firstCall to false, because this function should be executed once
	firstCall = FALSE;

	//creation of the first thread (main_thread)
	executingThread = tcb_const(0);			//MUDAR PARA PRIORIDADE BAIXA. UTILIZANDO PRIORIDADE
							//DIFERENTE DA ESTABELECIDA PARA FINS DE TESTES

	executingThread->next = NULL;
	
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

void dispatcher()
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

			if(auxThread->status == BLOCKED)
				insertThread(blockedList, auxThread);			//insert in blocked list
			
			else if(auxThread->status == APT)
				insertThread(aptList[auxThread->priority], auxThread);	//insert in apt list

			setcontext(&executingThread->context);
		}
	}

}

