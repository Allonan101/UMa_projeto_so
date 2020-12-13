#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>


#define NUM_PRODUTORES   1
#define NUM_CONSUMIDORES 1
#define NUM_ITEMS        45
#define TAMANHO          20

typedef struct {
  int buf[TAMANHO];
  sem_t ocupados;
  sem_t livres;
  int segdep;		
  int segret;
  sem_t pmut;
  sem_t cmut;
} buffer_t;

buffer_t buffer;

void *produtor (void *ptr) {
  int i,l,p,o;
  buffer_t *b = (buffer_t *) ptr;

      for (i = 1; i <= NUM_ITEMS; i++) {
    sem_wait(&b->livres);
    sem_wait(&b->pmut);
    b->buf[b->segdep] = 1000 * (int)pthread_self() + i;
	sem_getvalue(&b->livres,&l);
	sem_getvalue(&b->pmut,&p);
    printf ("Produtor %3d produziu um valor.\n Semaforo livres:%d e semaforo pmut:%d\n", pthread_self(),l,p);
    pthread_yield();
    sleep(1);
    b->segdep++;
    b->segdep %= TAMANHO;
    sem_post(&b->pmut);
    sem_post(&b->ocupados);
	sem_getvalue(&b->ocupados,&o);
	sem_getvalue(&b->pmut,&p);
    printf ("Produtor Semaforo ocupados:%d e semaforo pmut:%d\n\n",o,p);


  }
  return NULL;
}

void *consumidor (void *ptr) {
  int i,o,c,l;
  buffer_t *b = (buffer_t *) ptr;

  for (;;) {
    sem_wait(&b->ocupados);
    sem_wait(&b->cmut);
	sem_getvalue(&b->ocupados,&o);
	sem_getvalue(&b->cmut,&c);
    printf ("Consumidor %3d consumiu um valor. \nSemaforo ocupados:%d e semaforo cmut:%d\n", pthread_self(),o,c);
    pthread_yield();
    sleep(1);
    b->segret++;
    b->segret %= TAMANHO;
    sem_post(&b->cmut);
    sem_post(&b->livres);
	sem_getvalue(&b->livres,&l);
	sem_getvalue(&b->cmut,&c);
    printf ("Consumidor Semaforo livres:%d e semaforo cmut:%d\n\n",l,c);

  }
  return NULL;
}

int main()
{
  int i;
  pthread_t thread_id[NUM_PRODUTORES+NUM_CONSUMIDORES];

  sem_init (&buffer.ocupados, 0, 0);
  sem_init (&buffer.livres, 0, TAMANHO);
  sem_init (&buffer.pmut, 0, 1);
  sem_init (&buffer.cmut, 0, 1);
  buffer.segdep = buffer.segret = 0;

  for (i = 0; i < NUM_PRODUTORES; i++) {
    if (pthread_create(&thread_id[i],NULL,produtor,(void *)&buffer) != 0) {
      printf("erro na criacao da tarefa\n");
      exit(1);
    }
  }

  for (i = 0; i < NUM_CONSUMIDORES; i++) {
    if (pthread_create(&thread_id[i+NUM_PRODUTORES],NULL,consumidor,(void *)&buffer) != 0) {
      printf("erro na criacao da tarefa\n");
      exit(1);
    }
  }

  for (i = 0; i < NUM_PRODUTORES + NUM_CONSUMIDORES; i++)
    pthread_join (thread_id[i], NULL);

  return 0;
} 
