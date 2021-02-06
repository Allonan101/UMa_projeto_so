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

int postos[];

//TRINCOS
pthread_mutex_t mutex_utente;
pthread_mutex_t mutex_posto_testagem; //trinco para ponto testagem
pthread_mutex_t mutex_isolamento; //trinco para isolamento

//SEMAFOROS
sem_t fila_prio;	
sem_t fila_normal;
sem_t fila_isolamento_prio;
sem_t fila_isolamento_normal;

//Variaveis globais
int id_utentes = 0; //é o counter e o id do utente
int numUtentesIsolamento = 0;
int numUtentesInternamento = 0;
int numUtentesPontoTestagem = 0;
int numUtentesCentroTestagem = 0;
int numUtentesFilaNormal = 0;
int numUtentesFilaPriori = 0;
int tempo_universal = 0;
int iniciado = 0;

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
	int covid;
};

struct utente utentes[NUM_MAX_PESSOAS];

void adiciona_utente(int id, int prioridade, int posto_testagem, int sala_isolamento, int internamento, int saiu, int tipo_teste, int tempo, int covid) {
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
		utentes[i].covid = covid;
	}
}

void imprime_utentes() {
	printf("----Utentes----\n");
	int j;
	for (j = 0; j < NUM_MAX_PESSOAS; ++j) {
		if(utentes[j].id == NULL){
			break;
		}
		printf("ID: %d ", utentes[j].id);
		
		printf("Prioridade: %d ", utentes[j].prioridade);
		printf("Posto: %d ", utentes[j].posto_testagem);
		printf("Sala: %d ", utentes[j].sala_isolamento);
		printf("Internamento: %d ", utentes[j].internamento);
		printf("Saiu: %d ", utentes[j].saiu);
		printf("Teste: %d ", utentes[j].tipo_teste);
		printf("Tempo: %d ", utentes[j].tempo);
		printf("Covid: %d ", utentes[j].covid);
		printf("\n");
	}
	printf("--------\n\n");
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

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
void *gerar_utentes(void *arg){

	int probabilidade;
	char buffer[BUFFER_SIZE];


	pthread_mutex_lock(&mutex_utente);
	int id_pessoal = id_utentes;
	id_utentes++; //adiciona na variavel global de utentes
	pthread_mutex_unlock(&mutex_utente);

	//DEFINIR PRIORIDADE
	int prioridade = 0;
	//printf("Prob de ser priori: %d \n",config.PROB_UTENTE_SER_PRIO);
	probabilidade = random_num(0,100); 
	//printf("Prob que calhou: %d \n",probabilidade);
	if(probabilidade <= config.PROB_UTENTE_SER_PRIO){
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
		if(numUtentesFilaNormal<config.MAX_PESSOAS_FILA){ 
			numUtentesFilaNormal++;
		}
		else{
			printf("Utente %d saiu por fila normal cheia \n", id_pessoal);
			saiu = 1;
		}
	}

	else{

		//tem espaço na fila para criar uma pessoa prioritária
		if(numUtentesFilaPriori<config.MAX_PESSOS_FILA_PRIORITARIA){ 
			numUtentesFilaPriori++;
		}
		else{
			printf("Utente %d saiu por fila prioritaria cheia \n",id_pessoal);
			saiu = 1;
		}

	}

	pthread_mutex_unlock(&mutex_utente);

	//TEMPOS
	int tempo = tempo_universal;
	int tempo_no_centro = 0;
	int tempo_no_posto_de_testagem = 0; //tempo que demora no teste

	
	//COVID
	int covid = 0;

	//POSTO QUE FICOU
	int posto_pessoal = 0;

	adiciona_utente(id_pessoal, prioridade, NULL, NULL, NULL, saiu, tipo_teste, tempo, covid);
	

	//Simulação de entrar no centro
	
	while(1){

		while(tempo_universal == utentes[id_pessoal].tempo){ //ZAWARUDOO Enquanto o tempo universal nao muda, nao faz nada.
			
		}

		//Time has begun to move again 
		
		pthread_mutex_lock(&mutex_utente);
		tempo_no_centro++;
		pthread_mutex_unlock(&mutex_utente);
		
		utentes[id_pessoal].tempo = tempo_universal;

		//printf("Tempo do utente %d é %d \n",id_pessoal,tempo);
		//printf("Tempo universal é %d \n",tempo_universal);
		//printf("Tempo do utente %d no centro é %d \n \n",id_pessoal,tempo_no_centro);
	
		if(utentes[id_pessoal].saiu != 1){ //Se essa pessoa nao saiu 

			if(utentes[id_pessoal].prioridade == 1){ //Se essa pessoa tem prioridade

				if(utentes[id_pessoal].posto_testagem == 2){ //Se essa pessoa já teve no posto de testagem 

					printf("Utente %d já teve no posto de testagem e vai para o isolamento \n",id_pessoal);

					if(utentes[id_pessoal].sala_isolamento == 1){ //Se está no isolamento
						
						if(utentes[id_pessoal].internamento == 1){ //Se está no internamento
						
						}
						else{ //Se está não está no internamento

						}
					}
					else{ //Não está no isolamento

					}

				}

				if(utentes[id_pessoal].posto_testagem == 1){ //Está no ponto de testagem ------------------------------

					printf("Utente %d está no ponto de testagem \n",id_pessoal);

					//verificar teste do utilizador

					if(utentes[id_pessoal].tipo_teste == 0){ //utente faz teste convencional

						if(tempo_no_posto_de_testagem < config.TEMPO_TESTE_CONVENCIONAL){ //espera para fazer o teste convencional
							pthread_mutex_lock(&mutex_utente);
							tempo_no_posto_de_testagem++;
							pthread_mutex_unlock(&mutex_utente);
						}
						else{
							
							printf("O utente %d recebeu o resultado do teste convecional \n",id_pessoal);
							int random_covid;
							random_covid = random_num(0,100); //probabilidade de ter covid positivo
							if(random_covid <= config.PROB_UTENTE_TER_COVID){
								utentes[id_pessoal].covid= 1;
								printf("O utente %d tem covid positivo \n \n",id_pessoal);
							}
							else{
								printf("O utente %d nao tem covid \n", id_pessoal);
								postos[posto_pessoal] = 0; //posto fica livre
								utentes[id_pessoal].saiu = 1; //sai do centro
							}
						}
						
			
					}
					else{	//utente faz teste_rápido

						if(tempo_no_posto_de_testagem < config.TEMPO_TESTE_RAPIDO){ //espera para fazer o teste rápido
							pthread_mutex_lock(&mutex_utente);
							tempo_no_posto_de_testagem++;
							pthread_mutex_unlock(&mutex_utente);
						}
						else{
							printf("O utente %d recebeu o resultado do teste rápido \n",id_pessoal);
							int random_covid;
							random_covid = random_num(0,100); //probabilidade de ter covid positivo
							if(random_covid <= config.PROB_UTENTE_TER_COVID){
								utentes[id_pessoal].covid= 1;
								printf("O utente %d tem covid positivo \n \n",id_pessoal);
							}
							else{
								printf("O utente %d nao tem covid \n", id_pessoal);
								postos[posto_pessoal] = 0; //posto fica livre
								utentes[id_pessoal].saiu = 1; //sai do centro
							}
						}
					}
	
				}

				if(utentes[id_pessoal].posto_testagem == 0){ //Não está no ponto de testagem -------------------------

					//verificar se já esperou o tempo nas configurações 
					if(tempo_no_centro >= config.TEMPO_ATENDIMENTO){

						//verificar se o posto está cheio, se nao estiver entra com o semaforo
						sem_wait(&fila_prio); //===================================================================================
						int x;
						for(x = 0; x < config.NUMERO_POSTOS_TESTAGEM; x++){
							if(postos[x] == 0){
							postos[x] = 1;
							posto_pessoal = x;
							utentes[id_pessoal].posto_testagem = 1;
							printf("Utente %d entrou no posto de testagem %d \n",id_pessoal,x);
							numUtentesFilaPriori--;
							break;
							}
						}
						sem_post(&fila_prio); //===================================================================================
					}

				}
				
				
			}
			
			else{ //Não tem prioridade

				if(utentes[id_pessoal].posto_testagem == 2){ //Se essa pessoa já teve no posto de testagem 

					printf("Utente %d já teve no posto de testagem e vai para o isolamento \n",id_pessoal);

					if(utentes[id_pessoal].sala_isolamento == 1){ //Se está no isolamento
						
						if(utentes[id_pessoal].internamento == 1){ //Se está no internamento
						
						}
						else{ //Se está não está no internamento

						}
					}
					else{ //Não está no isolamento

					}

				}

				if(utentes[id_pessoal].posto_testagem == 1){ //Está no ponto de testagem ------------------------------

					printf("Utente %d está no ponto de testagem \n",id_pessoal);

					//verificar teste do utilizador

					if(utentes[id_pessoal].tipo_teste == 0){ //utente faz teste convencional
						
						if(tempo_no_posto_de_testagem < config.TEMPO_TESTE_CONVENCIONAL){ //espera para fazer o teste convencional
							pthread_mutex_lock(&mutex_utente);
							tempo_no_posto_de_testagem++;
							pthread_mutex_unlock(&mutex_utente);
						}
						else{
							
							printf("O utente %d recebeu o resultado do teste convecional \n",id_pessoal);
							int random_covid;
							random_covid = random_num(0,100); //probabilidade de ter covid positivo
							if(random_covid <= config.PROB_UTENTE_TER_COVID){
								utentes[id_pessoal].covid= 1;
								printf("O utente %d tem covid positivo \n \n",id_pessoal);
							}
							else{
								printf("O utente %d nao tem covid \n", id_pessoal);
								postos[posto_pessoal] = 0; //posto fica livre
								utentes[id_pessoal].saiu = 1; //sai do centro
							}
						}
						
			
					}
					else{	//utente faz teste_rápido

						if(tempo_no_posto_de_testagem < config.TEMPO_TESTE_RAPIDO){ //espera para fazer o teste rápido
							pthread_mutex_lock(&mutex_utente);
							tempo_no_posto_de_testagem++;
							pthread_mutex_unlock(&mutex_utente);
						}
						else{
							printf("O utente %d recebeu o resultado do teste rápido \n",id_pessoal);
							int random_covid;
							random_covid = random_num(0,100); //probabilidade de ter covid positivo
							if(random_covid <= config.PROB_UTENTE_TER_COVID){
								utentes[id_pessoal].covid= 1;
								printf("O utente %d tem covid positivo \n \n",id_pessoal);
							}
							else{
								printf("O utente %d nao tem covid \n", id_pessoal);
								postos[posto_pessoal] = 0; //posto fica livre
								utentes[id_pessoal].saiu = 1; //sai do centro
							}
						}
					}
	
				}

				if(utentes[id_pessoal].posto_testagem == 0){ //Não está no ponto de testagem -------------------------

					//verificar se já esperou o tempo nas configurações 
					if(tempo_no_centro >= config.TEMPO_ATENDIMENTO){

						//verificar se o posto está cheio, se nao estiver entra com o semaforo
						sem_wait(&fila_normal); //===================================================================================
						int x;
						for(x = 0; x < config.NUMERO_POSTOS_TESTAGEM; x++){
							if(postos[x] == 0){
							postos[x] = 1;
							posto_pessoal = x;
							utentes[id_pessoal].posto_testagem = 1;
							printf("Utente %d entrou no posto de testagem %d \n",id_pessoal,x);
							numUtentesFilaNormal--;
							break;
							}
						}
						sem_post(&fila_normal); //===================================================================================
					}

				}
			}
		}
		else{
			printf("Utente %d saiu. \n",id_pessoal);
			pthread_cancel(pthread_self()); 
		}
		
	}

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
	puts("Bind completo \n");

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

	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	//Receber mensagem do cliente
	while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
	{	
		
		int opcao = atoi(client_message);


		if(opcao == 1 && iniciado == 1){
			opcao = 2;
			sprintf(client_message, "%d", opcao); //int para str
		}
		if(opcao == 1 && iniciado == 0){
			iniciado = 1;
		}

		switch(opcao){
			case 1 : {  //Iniciar simulação

				//Enviar o evento da abertura do centro
				write(client_sock , client_message , strlen(client_message)+1);
				break;
			}

			case 2: { //Continuar a simulacao 
				
				int numero_random = random_num(1,1);
				int i = 0;
				for (i = 0; i < numero_random; i++){
					pthread_t thread;
					pthread_create(&thread, NULL, &gerar_utentes, NULL);
				}		
				
				//Função para gerar aleatoriamente um ou mais utentes com threads 

				write(client_sock , client_message , strlen(client_message)+1);
				imprime_utentes();
				tempo_universal++;

	
				break;
			}
			case 3: { //Pausar a simulacao

				
				break; 
			}
			case 5: {
				printf("Terminando com o codigo: %d \n",opcao);
				exit(0);
				break;
			}
		}

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


	printf("Load postos de testagem \n");
	int x;
	for(x = 0; x < config.NUMERO_POSTOS_TESTAGEM; x++){
		postos[x] = 0;
		printf("Posto %d criado com valor %d \n",x,postos[x]);
	}

	sem_init(&fila_normal,0,1);
	sem_init(&fila_prio,0,1);
	sem_init(&fila_isolamento_normal,0,1);
	sem_init(&fila_isolamento_prio,0,1);

	criasocket();

	sem_destroy(&fila_normal);
	sem_destroy(&fila_prio);
	sem_destroy(&fila_isolamento_normal);
	sem_destroy(&fila_isolamento_prio);
	

}