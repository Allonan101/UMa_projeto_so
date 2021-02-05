#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>	
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>	
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
#include <time.h>

#define NAME_SIZE 100
#define BUFFER_SIZE 256 
#define NUMBER_OF_CONFIGS 15
#define NUM_MAX_PESSOAS 20

//TRINCOS
pthread_mutex_t mutex_utente;
pthread_mutex_t mutex_posto_testagem; //trinco para ponto testagem
pthread_mutex_t mutex_isolamento; //trinco para isolamento

//SEMAFOROS
sem_t fila_prio;	
sem_t fila_normal;
sem_t isolamento_prio;
sem_t isolamento_normal;

int id_utentes = 0;
int utenteNormais = 0; 
int utentePriori = 0; 
int utentePrioriIsolamento = 0;
int utenteNormalIsolamento = 0;
int utenteInternamento = 0;
int utentePontoTestagem = 0;
int utenteCentroTestagem = 0;
int utenteNormalFila = 0;
int utentePrioriFila = 0;
int utenteIsolamFila = 0;
int tempo_universal = 0;


typedef struct {
	int TEMPO_MEDIO_CHEGADA_UTENTES;
	int NUMERO_POSTOS_TESTAGEM;
	int NUMERO_CENTROS_TESTAGEM;
	int NUMERO_MAX_TESTES_PESSOA;
	int MAX_PESSOAS_FILA;
	int MAX_PESSOS_FILA_PRIORITARIA;
	int PROB_UTENTE_TER_COVID;
	int PROB_UTENTE_SER_PRIO;
	int ATUALIZACAO_RESULTADO;
	int NUMERO_MAX_DOENTES_INTERNAMENTO;
	int NUMERO_MAX_ISOLAMENTO;
	int TEMPO_ATENDIMENTO;
	int TEMPO_ISOLAMENTO;
	int TEMPO_TESTE_RAPIDO;
	int TEMPO_TESTE_CONVENCIONAL;
}configuracao;

configuracao config;

struct utente {
	int id;
	int prioridade; //prioridade = 1 então é prioritario
	//se alguma destas está a 1 é o lugar onde se encontra o utente
	int posto_testagem;
	int sala_isolamento;
	int internamento;
	int saiu;
	int tipo_teste;
	int tempo;
	int tempo_global;
};

struct utente utentes[NUM_MAX_PESSOAS];

void adiciona_utente(int id, int prioridade, int posto_testagem, int sala_isolamento, int internamento, int saiu, int tipo_teste, int tempo, int tempo_global) {
	int i = 0;
	while (utentes[i].id != NULL && i<NUM_MAX_PESSOAS) {
		i++;
	}
	if (utentes[i].id == NULL) {
		utentes[i].id = id;
		utentes[i].prioridade = prioridade;
		utentes[i].posto_testagem = posto_testagem;
		utentes[i].sala_isolamento = sala_isolamento;
		utentes[i].internamento = internamento;
		utentes[i].saiu = saiu;
		utentes[i].tipo_teste = tipo_teste;
		utentes[i].tempo = tempo;
		utentes[i].tempo_global = tempo_global;
	}
}

void imprime_utentes() {
	printf("Utentes: \n");
	int j;
	for (j = 0; j < NUM_MAX_PESSOAS; ++j) {
		if(utentes[j].id == NULL){
			break;
		}
		printf("%d", utentes[j].id);
		printf("%d", utentes[j].prioridade);
		printf("%d", utentes[j].posto_testagem);
		printf("%d", utentes[j].sala_isolamento);
		printf("%d", utentes[j].internamento);
		printf("%d", utentes[j].saiu);
		printf("%d", utentes[j].tipo_teste);
		printf("%d", utentes[j].tempo);
		printf("%d", utentes[j].tempo_global);
		printf("\n");
	}
}

int random_num(int min, int max){
    return (rand() % max) + min;
}


int* load_conf_simulador(char *file)
{
	int* conf = (int*)malloc(sizeof(int) * NUMBER_OF_CONFIGS);
	FILE *fp = fopen(file, "rb");

	if (fp == NULL)
	{
		printf("Não e possível abrir o ficheiro de configuração.\n");
		abort();
	}

	int num, i = 0;
	char name[NAME_SIZE], buff[BUFFER_SIZE];

	while (fgets(buff, sizeof buff, fp) != NULL)
	{
		if (sscanf(buff, "%[^=]=%d", name, &num) == 2)
			conf[i++] = num;
	}

	fclose(fp);
	return conf;
}

void *utente(void *arg){
	/*
	config.TEMPO_MEDIO_CHEGADA_UTENTES;
	config.NUMERO_POSTOS_TESTAGEM;
	config.NUMERO_CENTROS_TESTAGEM;
	config.NUMERO_MAX_TESTES_PESSOA;
	config.MAX_PESSOAS_FILA;
	config.MAX_PESSOS_FILA_PRIORITARIA;
	config.PROB_UTENTE_TER_COVID;
	config.PROB_UTENTE_SER_PRIO;
	config.ATUALIZACAO_RESULTADO;
	config.NUMERO_MAX_DOENTES_INTERNAMENTO;
	config.NUMERO_MAX_ISOLAMENTO;
	config.TEMPO_ATENDIMENTO;
	config.TEMPO_ISOLAMENTO;
	config.TEMPO_TESTE_RAPIDO;
	config.TEMPO_TESTE_CONVENCIONAL;
	*/
	
	while(1){
		printf("Verificando utente : \n");

		int j;
		
			
		if(utentes[j].saiu != 1){ //Se essa pessoa nao saiu 
			
			if(utentes[j].prioridade == 1){ //Se essa pessoa tem prioridade

				if(utentes[j].posto_testagem == 1){ //Se está no ponto de testagem
					
					if(utentes[j].sala_isolamento == 1){ //Se está no isolamento
						
						if(utentes[j].internamento == 1){ //Se está no internamento
						
						}
						else{ //Se está não está no internamento

						}
					}
					else{ //Não está no isolamento

					}
				}
				else{ //Não está no ponto de testagem

				}
			}
			
			else{ //Não tem prioridade

				if(utentes[j].posto_testagem == 1){ //Se está no ponto de testagem
					
					if(utentes[j].sala_isolamento == 1){ //Se está no isolamento
						
						if(utentes[j].internamento == 1){ //Se está no internamento
						
						}
						else{ //Se está não está no internamento

						}
					}
					else{ //Não está no isolamento

					}
				}
				else{ //Não está no ponto de testagem

				}
			}
		}
		else{
			printf("Utente x saiu.");
			//pthread_cancel(pthread_t thread);
		}
			
			
		printf("%d", utentes[j].id);
		printf("%d", utentes[j].prioridade);
		printf("%d", utentes[j].posto_testagem);
		printf("%d", utentes[j].sala_isolamento);
		printf("%d", utentes[j].internamento);
		printf("%d", utentes[j].saiu);
		printf("%d", utentes[j].tipo_teste);
		printf("%d", utentes[j].tempo);
		printf("%d", utentes[j].tempo_global);
		printf("\n");
		
	}
}


void *gerar_utentes(void *arg){

	int probabilidade;

	pthread_mutex_lock(&mutex_utente);
	id_utentes++; //adiciona na variavel global de utentes
	pthread_mutex_unlock(&mutex_utente);

	//DEFINIR PRIORIDADE
	int prioridade = 0;
	//printf("Prob de ser priori: %d \n",config.PROB_UTENTE_SER_PRIO);
	probabilidade = random_num(1,config.PROB_UTENTE_SER_PRIO); 
	//printf("Prob que calhou: %d \n",probabilidade);
	if(probabilidade == config.PROB_UTENTE_SER_PRIO){
		prioridade = 1;
	}

	//DEFINIR TIPO DE TESTE 50/50
	int tipo_teste = 0;
	//printf("Prob de ser teste longo: 50/50 \n");
	probabilidade = random_num(1,2); 
	//printf("Prob que calhou: %d \n",probabilidade);
	if(probabilidade == 1){
		tipo_teste = 1;
	}


	//FILAS

	int saiu = 0;
	pthread_mutex_lock(&mutex_utente);

	if(prioridade == 0){

		//verificar se tem espaço na fila
		if(utenteNormalFila<config.MAX_PESSOAS_FILA){ 
			utenteNormalFila++;
			utenteNormais++;
		}
		else{
			printf("Utente saiu por fila normal cheia \n");
			saiu = 1;
		}
	}

	else{

		//tem espaço na fila para criar uma pessoa prioritária
		if(utentePrioriFila<config.MAX_PESSOS_FILA_PRIORITARIA){ 
			utentePrioriFila++;
			utentePriori++;
		}
		else{
			printf("Utente saiu por fila prioritaria cheia \n");
			saiu = 1;
		}

	}

	pthread_mutex_unlock(&mutex_utente);

	//TEMPOS
	int tempo = 0;
	int tempo_global = tempo_universal;

	adiciona_utente(id_utentes, prioridade, NULL, NULL, NULL, saiu, tipo_teste, tempo, tempo_global);
}


void criasocket(){

	int socket_desc , client_sock , c , read_size;
	struct sockaddr_in server , client;
	char client_message[2000];
	
	//Criar socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Não foi possivel criar o socket");
	}
	puts("Socket criado");

	//Preparar sockaddr_in na estrutura
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8888 );

	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("Bind falhou");
		return 1;
	}
	puts("Bind completo");

	//Listen
	listen(socket_desc , 3);

	//Aceitar conexões
	puts("Esperando conexões...");
	c = sizeof(struct sockaddr_in);

	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if (client_sock < 0)
	{
		perror("Falha ao aceitar");
		return 1;
	}
	puts("Conexão aceite");

	int iniciado = 0;

	//Receber mensagem do cliente
	while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
	{	

		int opcao = atoi(client_message);

		if(opcao == 1 && iniciado == 1){
			opcao = 2;
		}
		if(opcao == 1 && iniciado == 0){
			iniciado = 1;
		}

		switch(opcao){
			case 1 : {  //Iniciar simulação


				//Função para gerar aleatoriamente um ou mais utentes com threads 
				pthread_t thread;
				pthread_create(&thread, NULL, &gerar_utentes, NULL);
				imprime_utentes();
				
				//Enviar o evento da abertura do centro
				write(client_sock , client_message , strlen(client_message)+1);
				break;
			}

			case 2: { //Continuar a simulacao 
				tempo_universal++;

				//Função para entrar no centro
				break;
			}
			case 3: { //Pausar a simulacao

				
				break; 
			}
			case 5: {
				printf("Terminando com o codigo: %d",opcao);
				exit(0);
				break;
			}
		}


		//Enviar mensagem de volta ao cliente
		write(client_sock , client_message , strlen(client_message)+1);

		//----------------Comunicação-------------------
		
		//Tempo ++

	}
	
	if(read_size == 0)
	{
		puts("Cliente desconectou");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv falhou");
	}

	return 0;

}
int main(int argc, char **argv) {

	//LOAD CONFIGURAÇÃO
	int *conf = load_conf_simulador(argv[1]);
	config.TEMPO_MEDIO_CHEGADA_UTENTES = conf[0];
	config.NUMERO_POSTOS_TESTAGEM = conf[1];
	config.NUMERO_CENTROS_TESTAGEM = conf[2];
	config.NUMERO_MAX_TESTES_PESSOA = conf[3];
	config.MAX_PESSOAS_FILA = conf[4];
	config.MAX_PESSOS_FILA_PRIORITARIA = conf[5];
	config.PROB_UTENTE_TER_COVID = conf[6];
	config.PROB_UTENTE_SER_PRIO = conf[7];
	config.ATUALIZACAO_RESULTADO = conf[8];
	config.NUMERO_MAX_DOENTES_INTERNAMENTO = conf[9];
	config.NUMERO_MAX_ISOLAMENTO = conf[10];
	config.TEMPO_ATENDIMENTO = conf[11];
	config.TEMPO_ISOLAMENTO = conf[12];
	config.TEMPO_TESTE_RAPIDO = conf[13];
	config.TEMPO_TESTE_CONVENCIONAL = conf[14];

	printf ("Load das configurações:\n");
	int loop;
	for (loop = 0; loop < NUMBER_OF_CONFIGS; loop++){
		printf("%d ", conf[loop]);
	}
	printf ("\n");

	criasocket();

	

}