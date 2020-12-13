#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N       5 
#define TAMANHO 10

int indice;
pthread_mutex_t excmut;

void *preenche (void *ptr) {
  int *b = (int *) ptr;

  while (indice < TAMANHO) {
    pthread_mutex_lock(&excmut); 
    b[indice] = 1;
    indice++;
    pthread_mutex_unlock(&excmut); 
    //pthread_yield();
    
  }
}

main()
{
  int i, j;
  int buffer[N][TAMANHO];
  pthread_t thread_id[N];
  pthread_mutex_init (&excmut, NULL);

  for (i = 0; i < N  ; i++) 
    memset (buffer[i], 0, sizeof(int) * TAMANHO);
  indice = 0;

  for (i = 0; i < N; i++) {
    if (pthread_create(&thread_id[i],NULL,preenche,(void *)&(buffer[i]))!=0) {
      printf("erro na criacao da tarefa\n");
      exit(1);
    }
  }

  for (i = 0; i < N; i++)
    pthread_join (thread_id[i], NULL);

  for (i = 0; i < N; i++) {
    for (j = 0; j < TAMANHO; j++)
      printf ("%d", buffer[i][j]);
    printf ("\n");
  }
} 
