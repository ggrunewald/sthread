#ifndef __sdata__
#define __sdata__

#include <ucontext.h>

#define FALSE 0
#define TRUE 1

#define SUCCESS 0
#define ERROR -1

#define	APT 0			//may be executed
#define	BLOCKED 1		//cant be executed
#define	EXECUTING 2		//being executed
#define	CREATED 3		//recently created
#define	ENDED 4			//already executed and ended

typedef struct tcb 
{
	int status;		//thread's status can be any of the above enum
	int tid;		//thread's identification number
	int priority;		//thread's execution priority
	ucontext_t context;	//context that will be setted when this thread's status became executing
	char stack[SIGSTKSZ];	//thread's stack
	struct tcb * next;	//next TCB element of the queue
}tcb;
 
typedef struct mutex 
{
	int flag;		//if it is busy
    	struct tcb *first;	//first element of blocked list
	struct tcb *last;	//last element of blocked list
} smutex_t;


tcb * tcb_const(int p);			//constructor default TCB
void tcb_dest(tcb * thread);		//destructor TCB
smutex_t* mutex_const();		//constructor default mutex
void mutex_dest(smutex_t* mtx);		//destructor mutex


int dispatcherInit();			//initializes the lists and other necessary structures


tcb * executingThread;			//thread at the ucp
ucontext_t returnContext;		//return thread must return after its end

struct threadList * aptList[3];
struct threadList * blockedList;		//blocked threads (cant be executed)

#endif
