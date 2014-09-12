#ifndef __sdata__
#define __sdata__

#include <ucontext.h>

//#include "list.h"

#define TRUE 1
#define FALSE 0

enum
{
	apt,			//may be executed
	blocked,		//cant be executed
	executing,		//being executed
	created,		//recently created
	ended			//already executed and ended
};


typedef struct tcb 
{
	int status;		//thread's status can be any of the above enum
	int tid;		//thread's identification number
	ucontext_t context;	//context that will be setted when this thread's status became executing
	char stack[SIGSTKSZ];	//thread's stack
	struct tcb * next;	//next TCB element of the queue
 } TCB;
 
typedef struct mutex 
{
	int flag;		//if it is busy
    	TCB *first;		//first element of blocked list
	TCB *last;		//last element of blocked list
} smutex_t;


TCB * tcb_const();			//constructor default TCB
void tcb_dest(TCB* thread);		//destructor TCB
smutex_t* mutex_const();		//constructor default mutex
void mutex_dest(smutex_t* mtx);		//destructor mutex


void dispatcherInit(void);		//initializes the lists and other necessary structures


TCB * executingThread;			//thread at the ucp
ucontext_t returnContext;		//return thread must return after its end

struct threadList * aptList[3];
struct threadList * blockedList;	//blocked threads (cant be executed)

int threadCounter;			//thread counter (usefull for identification)

#endif
