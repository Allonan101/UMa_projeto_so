
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/* constantes */
#define LIMITE 1000000

/* variaveis globais */
int var_partilhada;
pthread_mutex_t mutex;
pthread_t t1, t2;


void* tarefa1(void* ptr) {
	int quit = 0;

    while(!quit){
        pthread_mutex_lock(&mutex);
        printf("Tarefa 1 var_partilhada (antes)  = %d\n",
                var_partilhada);
        var_partilhada *= 2;
        printf("Tarefa 1 var_partilhada (depois) = %d\n",
                var_partilhada);
        if(var_partilhada > LIMITE)
        	quit = 1;
        pthread_mutex_unlock(&mutex);
        printf("Tarefa 1 var_partilhada (aberto) = %d\n",
                var_partilhada);
        sleep(1);
    }

	return NULL;
}


void* tarefa2(void* ptr) {
	int quit = 0;

    while(!quit) {
        pthread_mutex_lock(&mutex);
        printf("Tarefa 2 var_partilhada (antes)  = %d\n",
                var_partilhada);
        var_partilhada += 40;
        printf("Tarefa 2 var_partilhada (depois) = %d\n",
                var_partilhada);
        if(var_partilhada > LIMITE)
        	quit = 1;
        pthread_mutex_unlock(&mutex);
        printf("Tarefa 2 var_partilhada (aberto) = %d\n",
                var_partilhada);
        sleep(1);
    }
	
	return NULL;
}


int main() {

    /* iniciacao das variaveis globais */
    var_partilhada = 1000;
    pthread_mutex_init(&mutex, NULL);
    printf("Valor inicial de var_partilhada: %d \n",
            var_partilhada);

    printf("Criar tarefas\n");
    pthread_create(&t1, NULL, &tarefa1, NULL);
    pthread_create(&t2, NULL, &tarefa2, NULL);

    printf("Esperar que terminem\n");
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

	pthread_mutex_destroy(&mutex);

	return 0;
}
