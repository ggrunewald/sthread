/*

	A ideia desse teste é criar uma thread na main que aponta para function1.
	Na function1, cria-se mais MAX_THREADS threads, cada uma redirecionando para function2.
	Na function2, cria-se mais MAX_THREADS threads, cada uma redirecionando para function3.
	Na function3, cada thread imprime um valor no visor. Esse valor é incrementado a cada thread que é criada.

*/


#include "sthread.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 10

smutex_t mutex1, mutex2;
int threadCount = 0;

void function3()
{
	syield();
	printf("%d\n", threadCount);
}

void function2()
{
	int threads[MAX_THREADS];
	int i = 0;
	
	for (i = 0; i < MAX_THREADS; i++)
	{
		slock(&mutex2);
		threadCount++;
		sunlock(&mutex2);
		threads[i] = screate(1, &function3, NULL);
		syield();
	}

}

void function1()
{
	int threads[MAX_THREADS];
	int i = 0;

	for (i = 0; i < MAX_THREADS; i++)
	{
		slock(&mutex1);
		threadCount++;
		sunlock(&mutex1);
		threads[i] = screate(1, &function2, NULL);
	}

	for (i = 0; i < MAX_THREADS; i++)
	{
		int x = swait(threads[i]);
	}

}

int main ()
{
	int mainThread;
	smutex_init(&mutex1);
	smutex_init(&mutex2);

	mainThread = screate(2, &function1, NULL);

	swait(mainThread);

	
	return 0;
}
