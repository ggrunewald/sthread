#ifndef __functions__
#define __functions__

int insert(smutex_t *mtx, TCB * thread);

TCB * remove(smutex_t *mtx);

TCB * search(smutex_t *mtx, int id);

#endif
