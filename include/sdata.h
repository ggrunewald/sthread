/*
 * sdata.h: arquivo de inclus�o onde os grupos devem colocar as defini��es
 *          de suas estruturas de dados
 *
 * VERS�O 1 - 20/08/2014
 */
 
 /*
  * Exemplo de estrutura de dados da Thread Control Block
  */
 typedef struct tcb {
	int estado;
	// ....
	struct tcb *next;
 } TCB;
 
/*
 * Exemplo de estrutura de dados "mutex"
 * Os grupos devem alterar essa estrutura de acordo com sua necessidade
 */ 
typedef struct mutex {
	int flag;
    TCB *first;
	TCB *last;
} mmutex_t;
