//PRODUTORES E CONSUMIDORES
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "queue.c"	// buffer compartilhado
#include <string.h>
#include <sys/time.h>

char *matchWord;
int *occurr_found;
int match(char *procurado, char *linha );
void *produtor( void *NomeArq); 
void *consumidor( void  *id );
int match(char *procurado, char *linha );
 
 
void *produtor( void *NomeArq){
	
	FILE *arq;
	if((arq=fopen(NomeArq , "r"))==NULL)	// Abre arquivo
	{
		printf("Error in file open!\n");
		exit(1);
	}
	char linha[133];
	
	// Adiciona as linhas no buffer enquanto não estiver no fim do arquivo
	while(1){
		fgets(linha, 133, arq);
		if (!feof(arq))
			insert(linha);	// insere atomicamente no buffer
		else break;
	}
	
	fgets(linha, 1, arq);
	insert(linha);			// Adiciona caractere de fim de arquivo
	fclose(arq);
	pthread_exit(NULL);
}

/*
 * Consumidor pega uma posicao da fila por iteração;
 * */
void *consumidor( void  *id ){
	int tid = *((int *)(id));
	int found=0;	
	for(; ;){
		char *linha;
		linha = get();	// Comsumidor pega atomicamente uma linha do buffer.
		
		if(linha[0] == 0){ 	// Mata a thread caso seja lido o caractere de fim de arquivo.
			occurr_found[tid] = found;
			insert(linha);			/* Adiciona o caracter de fim de arquivo novamente no 
										buffer para as outras threads pegarem. */
			pthread_exit(NULL);			
		}
		found += match(matchWord, linha);	// Atualiza número de ocorrências encontradas.
	}
	pthread_exit(NULL);
}


/*
 *	Função para pesquisar ocorrências em uma linha. 
 */
int match(char *string, char *line ){
	
	int i,j,k, found=0;
	
	// Aqui percorre todos caracteres da linha e da string, comparando-os
	for(i = 0; line[i]; i++)
	{
		for(j = i, k = 0; line[j] == string[k]; j++, k++)
		{
			if(string[k + 1] == 0) // fim da comparação de uma ocorrência
			{
				found++;
			}
		}
	}
	
	return found;
}

int main (int argc, char* argv[]){
	
	int cores = sysconf(_SC_NPROCESSORS_ONLN);	// cria número de threads igual ao número de processadores.
	
	pthread_t consumidores[cores], prod ; // comsumidores (x Número de Processadores) e o produtor
	pthread_attr_t thread_attr;	// atributos das threads que serão criadas
	
	matchWord = argv[2];
	int flag;	
	int i, id_threads[cores];
	
	// Inicializa atributos da thread
	flag = pthread_attr_init(&thread_attr);
	if (flag!=0) printf("Erro na criação dos atributos das threads");
	
	// Seta o atributo das threads comsumidoras para serem Threads de Kernel, e não de usuário.
	flag = pthread_attr_setscope(&thread_attr, PTHREAD_SCOPE_SYSTEM);
	if (flag!=0) printf("Erro na criação do escopo da thread");
	
	
	occurr_found = malloc(sizeof(int)*cores);	 // aloca dinamicamente um espaço global para armazenamento de ocorrencias
	
	
	for(i=0;i<cores;i++)
	{
		id_threads[i]=i;		// Cria um vetor de identificação para as threads.
	}
	
	
	char *nomearq;
	nomearq = argv[1];	// Carrega o nome do arquivo
	
	pthread_create(&prod, NULL, produtor, (void *) nomearq );	// Cria o produtor
	
	
	// Cria as threads comsumidoras
	for(i=0; i< cores; i++){
		flag = pthread_create(&consumidores[i], &thread_attr, consumidor,(void *) &id_threads[i] );
		if (flag!=0) printf("Erro na criação da thread %i", i);
	}
	
	
	for(i=0; i< cores; i++){
		pthread_join(consumidores[i], NULL);	// Espera threads comsumidoras
	}
	
	pthread_join(prod, NULL);					// Espera produtor
	
	int occur_total=0;
	
	// Soma numero de ocorrencias que cada thread comsumidora encontrou.
	for (i=0;i<cores;i++)
	{
		//printf("%d: %d\n", i, occurr_found[i]);
		occur_total += occurr_found[i];				// conta número de ocorrências 
	}
	
	printf("%d", occur_total);
	
	return 0;
}
