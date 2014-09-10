#ifndef __functions__
#define __functions__

TCB * TCB_const(int id);

void TCB_dest(TCB* thread);

smutex_t* mutex_const();

void mutex_dest(smutex_t* mtx);

int mutex_initiallizer(smutex_t *mtx);

int insert_in_blockqueue(smutex_t *mtx, TCB * thread);

TCB * remove_from_blockqueue(smutex_t *mtx);

TCB * search_in_blockqueue(smutex_t *mtx, int id);

#endif
