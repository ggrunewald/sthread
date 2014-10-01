
/*
 *	Programa de exemplo de uso da biblioteca sthread
 *
 *	Versão 1.0 - 20/08/2014
 *
 *	Sistemas Operacionais I - www.inf.ufrgs.br
 *
 */

#include "../include/sthread.h"

#include <stdio.h>

void func0(void *arg) 
{

        printf("Eu sou a thread ID%d imprimindo\n", *((int *)arg));
	return;
}

void func1(void *arg) 
{

      printf("Eu sou a thread ID%d imprimindo\n", *((int *)arg));
}

int main(int argc, char *argv[]) 
{
    	int id1, id2, id3, id4;

    	int i = 1, j = 2, k = 3, l = 4;

    	id1 = screate(1, func0, (void *)&i);
    	id2 = screate(0, func1, (void *)&j);
	id3 = screate(2, func0, (void *)&k);
    	id4 = screate(0, func1, (void *)&l);

	int id5, id6, id7, id8;
	int a = 5, b=6, c=7, d=8;

	id5 = screate(2, func0, (void *)&a);
    	id6 = screate(0, func1, (void *)&b);
	id7 = screate(1, func0, (void *)&c);
    	id8 = screate(1, func1, (void *)&d);

    	printf("Eu sou a main depois da criacao das threads\n");

	swait(id4);
	swait(id6);
	swait(id7);
	swait(id8);
	swait(id1);
	swait(id2);
	swait(id3);
	swait(id5);

    	printf("Eu sou a main voltando para terminar o programa\n");

	return 0;
}

