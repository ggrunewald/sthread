/*
 * sthread.h: arquivo de inclus�o com os prot�tipos das fun��es a serem
 *            implementadas na realiza��o do trabalho.
 *
 * N�O MODIFIQUE ESTE ARQUIVO.
 *
 * VERS�O 1 - 20/08/2014
 */
#ifndef __sthread__
#define __sthread__

#include "sdata.h"
int screate (int prio, void (*start)(void*), void *arg);
int syield(void);
int swait(int tid);
int smutex_init(smutex_t *mtx);
int slock (smutex_t *);
int sunlock (smutex_t *);

#endif
