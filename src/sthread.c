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
	//puts("CONTEXTO SALVO 1");

	//modify de newThread context to return to the context pointed by returnContext
	newThread->context.uc_link = &returnContext;		
	newThread->context.uc_stack.ss_sp = newThread->stack;
	newThread->context.uc_stack.ss_size = sizeof(newThread->stack);

	//modify de newThread context to execute the start function
	makecontext(&newThread->context, (void*)(*start), 1, arg);

	newThread->status = APT;

	//insert the newThread in the apt list of its priority
	insertThread(aptList[newThread->priority], newThread);

	return aptList[newThread->priority]->last->tid;
}

//current executing thread liberates the cpu
//and is inserted at the apt list
int syield()
{
	executingThread->status = APT;
	int i = 0;
	//puts("SYIELD 1");
	
	getcontext(&executingThread->context);

	////printf("    %d      \n\n\n\n\n", i);

	if(i == 0)
	{
		i++;
		dispatcher(FALSE);
	}

	return SUCCESS;
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
	//puts("CONTEXTO SALVO 2");
	//sleep(1);

	int ret = 0;

	//puts("SWAIT 1");

	if(executingThread->tid != callerThread->tid)			//if its the waited thread (it already terminated its execution)
	{
		//puts("SWAIT 2");
		//sleep(1);
		if(executingThread->tid == waitedThread->tid)
		{
			//puts("SWAIT 3");
			//sleep(1);
			ret = 1;
//////printf("swait a exec = %d | ", executingThread->tid);
			callerThread->status = APT;				//make the waiter APT again

			removeThread(blockedList, callerThread->tid);		//remove it from the blocked list

			insertThread(aptList[callerThread->priority], callerThread);

//////printf("swait b exec = %d | ", executingThread->tid);
//////printf("swait b [0] = %d | ", aptList[0]->count);
//////printf("swait b [1] = %d | ", aptList[1]->count);
//////printf("swait b [2] = %d | ", aptList[2]->count);

		}
		//////printf("TIIIIIIIIIID %d\n", executingThread->tid);
		executingThread->status = ENDED;			//set its status to ended (will not run again)
		//////printf("STAAAAAAAAAAATUS %d\n", executingThread->status);

		//puts("SWAIT 4");
		//sleep(1);
		dispatcher(FALSE);
	}

	else if(executingThread->tid == callerThread->tid)
	{
		//puts("SWAIT 5");
		//sleep(1);
		while(TRUE)
		{
			//puts("SWAIT 6");
			//sleep(1);
			//quits the eternal loop when the waited thread have terminated
			if(executingThread->tid == waitedThread->tid && ret == 1)
				break;

			//puts("SWAIT 7");
			//sleep(1);
			dispatcher(FALSE);
		}
	}

	//puts("SWAIT 8");
	//sleep(1);

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
//////printf("SLOCK INICIO!\n");

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

	//////printf("SLOCK FIM!\n");

	return SUCCESS;
}

int sunlock (smutex_t *mtx)
{
	//////printf("SUNLOCK INICIO!\n");

	if(!mtx->locked)
	{
		//////printf("SUNLOCK FIM ERROR!\n");
		return ERROR;
	}

	mtx->locked = FALSE;

	//////printf("SUNLOCK 1!\n");

	//////printf("que0\n");
	mtx;
	//////printf("que1\n");
	mtx->blockList;
	//////printf("que2\n");
	mtx->blockList->count;
	//////printf("que3\n");

	if(mtx->blockList->count > 0)
	{
	//////printf("SUNLOCK 2!\n");
		tcb * thread;

		thread = mtx->blockList->first;
	//////printf("SUNLOCK 3!\n");
		removeThread(mtx->blockList, mtx->blockList->first->tid);
	//////printf("SUNLOCK 4!\n");
		thread->status = APT;
	//////printf("SUNLOCK 5!\n");
		insertThread(aptList[thread->priority], thread);
	}

	////////printf("SUNLOCK FIM!\n");

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
	//puts("CONTEXTO SALVO 3");

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

	//puts("DISPATCHER 1");

	for(i = 0; i < 3; i++)	
	{
		//puts("DISPATCHER 2");
		if(aptList[i]->count > 0)
		{
			//puts("DISPATCHER 3");
			executingThread = aptList[i]->first;				//gets next executed thread
			////printf("aptList->first: %d\n", aptList[i]->first->tid);

				//////printf("exec tid = %d ", executingThread->tid);
				//////printf("exec status = %d ", executingThread->status);
				//////printf("aux tid = %d ", auxThread->tid);
				//////printf("aux status = %d ", auxThread->status);
				//////printf("disp 2 [0] = %d | ", aptList[0]->count);
				//////printf("disp 2 [1] = %d | ", aptList[1]->count);
				//////printf("disp 2 [2] = %d\n", aptList[2]->count);

			executingThread->status = EXECUTING;

			removeThread(aptList[i], executingThread->tid);


			if(auxThread->status == BLOCKED && !isMutex)
			{
				//puts("DISPATCHER 4");
				insertThread(blockedList, auxThread);			//insert in blocked list

				setcontext(&executingThread->context);//swapcontext(&auxThread->context, &executingThread->context);
			}
			else if(auxThread->status == BLOCKED && isMutex)
			{
				//puts("DISPATCHER 5");
				setcontext(&executingThread->context);//swapcontext(&auxThread->context, &executingThread->context);
			}
			else if(auxThread->status == APT)
			{
				//puts("DISPATCHER 6");
				//////printf("TESTE APT i = %d e thread = %d\n", i, auxThread->tid);
				insertThread(aptList[auxThread->priority], auxThread);	//insert in apt list

				setcontext(&executingThread->context);//swapcontext(&auxThread->context, &executingThread->context);
			}
			else if(auxThread->status == ENDED)
			{
				//puts("DISPATCHER 7");
				//////printf("TESTE ENDED i = %d e thread = %d\n", i, auxThread->tid);
				auxThread = NULL;
//////printf("\n\n %d \n\n", executingThread->tid);
				setcontext(&executingThread->context);
			}
			else if(auxThread->status == EXECUTING)
			{

			}
		}
	}
}

