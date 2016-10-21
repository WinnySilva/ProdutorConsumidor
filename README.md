
# sisop_2015_2_t1 time 07
Trabalho 1 de SO 2015/2

## Integrantes do grupo:
Simão Schiller Martin  
Wine Dos Santos Silva

## Implementação do Trabalho 

Nosso trabalho 1 foi realizado em C usando pthreads. Para resolver o problema do produtor-consumidor, em nosso trabalho é criada uma thread produtora e para cada processador da máquina é criada uma thread consumidora. A thread produtora armazena as linhas lidas do arquivo em um buffer compartilhado. Esse buffer consiste em uma fila, onde cada posição dessa fila é uma linha do arquivo. Após inserido algo no buffer, as threads consumidoras começam a pegar atomicamente cada posição do buffer. Quando a thread produtora chegar ao fim do arquivo, adiciona um caractere especial de fim de arquivo no buffer, uma thread consumidora ao pegar esse caractere, coloca-o de volta (para outras threads pegarem) e encerra sua execução.
 
## Resolução de problemas de paralelização e sincronização

Solucionamos o problema de a identificação das threads estarem erradas passando uma posição de um vetor de identificação como parâmetro na criação das threads. Antes passávamos uma única variável "i" como parâmetro, o que deixava as identificações erradas.  
Percebemos que a execução do programa era mais lenta quando criávamos mais threads. Amenizamos o problema criando threads consumidoras de Kernel, ao invés de threads de Usuário. Assim pudemos explorar melhor o paralelismo.  
Para resolver a atomicidade das operações de colocar e retirar linhas no buffer, criamos uma variável do tipo *pthread_mutex_t* que é um mutex, bloqueando ele no início das funções *insert()* e *remove()* e desbloqueando ele no fim das mesmas.   
Para resolver o problema de quando o buffer está vazio ou cheio, criamos 2 variáveis de condição do tipo *pthread_cond_t*. Por exemplo, caso uma thread for pegar uma linha do buffer e este estiver vazio, essa thread é bloqueada na variável de condição "vazia" no mutex tipo *pthread_mutex_t*. Ao ter uma linha colocada no buffer, manda-se um *signal* à variável de condição e a thread bloqueada poderá seguir livremente.

## Resultados de experimentos

Os experimentos foram realizados em um computador quad-core com sistema operacional Linux Ubuntu.
Foram realizados 5 testes para cada caso e foi tirada a média aritmética deles.

- Caso de teste 1: "Teste1.txt" com pesquisa pela palavra "*Santo*". Número de ocorrências: 15.

	Threads     |   1   |   2   |   3   |   4   |
	------------|-------|-------|-------|-------| 
	ExecTime(ms)|   1   |   1   |   2   |   2   |
	SpeedUp     |   1   |   1   |   0.5 |  0.5  |

- Caso de teste 2: "Teste2.txt"com pesquisa pela palavra "*// Comsumidor pega atomicamente uma linha do buffer.*". Número de ocorrências: 334.

	Threads     |   1   |   2   |   3   |   4   |
	------------|-------|-------|-------|-------| 
	ExecTime(ms)|   36  |   45  |   90  |  115  |
	SpeedUp     |   1   |  0.8  |  0.4  | 0.31  |

- Caso de teste 3: "Teste3.txt"com pesquisa pela palavra "*Layout*". Número de ocorrências: 8888.

	Threads     |   1   |   2   |   3   |   4   |
	------------|-------|-------|-------|-------| 
	ExecTime(ms)|   94  |   80  |  130  |   160 |
	SpeedUp     |   1   | 1.18  |  0.72 |  0.59 |

- Caso de teste 4: "Teste4.txt"com pesquisa pela palavra "*aposdjkaspdkapsdklaskd plasdkalskdplaskdoqwdk*". Número de ocorrências: 314944.

	Threads     |   1   |   2   |   3   |   4   |
	------------|-------|-------|-------|-------| 
	ExecTime(ms)|  394  |  332  |  325  |   305 |
	SpeedUp     |   1   |  1.18 |  1.21 |  1.29 |

- Caso de teste 5: "Teste5.txt"com pesquisa pela palavra "*kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk*" (80 k's). Número de ocorrências: 32379776.

	Threads     |   1   |   2   |   3   |   4   |
	------------|-------|-------|-------|-------| 
	ExecTime(ms)| 21100 | 11572 | 11501 | 11463 |
	SpeedUp     |   1   |  1.82 |  1.83 | 1.84  |


###Link do Google Drive para os casos de teste usados:

https://drive.google.com/open?id=0B__tKpQ3joVmSWtYS2RTaWVtR2c
