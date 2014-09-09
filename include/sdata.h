#ifndef __sdata__
#define __sdata__

/*
 * Estrutura de dados da Thread
 */
typedef struct tcb 
{
	/*
	 * Enumerador dos estados possíveis
 	*/
	enum State
	{
		apt,
		blocked,
		executing,
		criated,
		ended
	}state;
	
	int tid;		//thread identification
	struct tcb *next;	//next node of queue
}TCB;
 
/*
 * Estrutura de dados mutex
 */ 
typedef struct mutex 
{
	int flag;	//is acquired
    	TCB *first;	//first blocked queue element
	TCB *last;	//last blocked queue element
}smutex_t;

#endif
