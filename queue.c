//fila de strings

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#define SIZE 500000

int size = SIZE, start = 0,end=0, elem=0;
char queue[SIZE][133];
//pthread_cond_t  conditionVariableA = PTHREAD_COND_INITIALIZER; // Variável de condição
pthread_cond_t  conditionCheio = PTHREAD_COND_INITIALIZER;
pthread_cond_t  conditionVazio = PTHREAD_COND_INITIALIZER;
pthread_mutex_t bufferMutex = PTHREAD_MUTEX_INITIALIZER; // Mutex para o buffer

/*
 * Função para inserir atomicamente uma linha no buffer. 
 * 
*/	
void insert(char *linha ){ 
	
	// Bloqueia o mutex no buffer
	pthread_mutex_lock (&bufferMutex);
	
	// Aqui enquanto o buffer estiver cheio, a thread espera na fila para colocar algo (até haver signal).
	while(elem==size)  pthread_cond_wait (&conditionCheio, &bufferMutex);
	
	strcpy(queue[end],linha);	
	end++;
	elem++;
	if(end == size){
		end = 0;	
	}
	
	// Manda signal para liberar uma thread na fila da variável de condição "Vazio" e desbloqueia o mutex
	pthread_cond_signal(&conditionVazio);
	pthread_mutex_unlock(&bufferMutex);
}
	
/*
 * Função que retorna atomicamente uma posição do buffer (1 linha)
 * 
*/	
char * get(){
	
	// Bloqueia o mutex no buffer
	pthread_mutex_lock(&bufferMutex);
	
	// Caso o buffer esteja vazio, a thread vai para a fila da variável de condição "Vazio" e espera ter algo no buffer
	while(elem == 0) pthread_cond_wait (&conditionVazio, &bufferMutex);
	
	elem--;
	char *qu;
	qu = queue[start];
	start++;
	if(start == size){
			start = 0;
	}
	
	// Manda signal para liberar uma thread na fila da variável de condição "Cheio" e desbloqueia o mutex
	pthread_cond_signal(&conditionCheio);
	pthread_mutex_unlock(&bufferMutex);
	
	return qu;
		
	}
