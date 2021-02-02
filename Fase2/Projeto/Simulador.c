#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>	
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>	

#define NAME_SIZE 100
#define BUFFER_SIZE 256 
#define NUMBER_OF_CONFIGS 15

/*					REVER
//semaforo que trata da fila prioritária e normal
//semaforo que trata do isolamento de utente prioritário e normal
sem_t fila_prio;
sem_t fila_normal;
sem_t isolamento_prio;
sem_t isolamento_normal;
*/

//trinco para ponto testagem
//trinco para isolamento
pthread_mutex_t mutex_posto_testagem;
pthread_mutex_t mutex_isolamento;

int utenteNormais = 0; 
int utentePriori = 0; 
int utentePrioriIsolamento = 0;
int utenteNormalIsolamento = 0;
int utenteInternamento = 0;
int utentePontoTestagem = 0;
int utenteCentroTestagem = 0;
int utenteIsolamento = 0;
//int numPontosTestagem = 0;
//int numCentroTestagem = 0;

struct configuracao
{
	int TEMPO_MEDIO_CHEGADA_UTENTES;
	int NUMERO_PONTOS_TESTAGEM;
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
};

/*					REVER
int *load_conf_simulador(char file){ //Load do icheiro de configuracao_simul.conf para listas
    int conf = (int*)malloc(sizeof(int) * NUMBER_OF_CONFIGS);
    FILE* fp = fopen(file, "rb");

    if (fp == NULL)
    {
        printf("Não é possível abrir o ficheiro de configuração.\n");
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
*/

/*					REVER
//Função para tratar o tipo de utente, utente prioritário=1,utente normal=2
void *tarefa_utente()
{
	utenteCentroTestagem++;
	int tipoUtente;
	int idUtente; 
	int PontoTestagemX = 0;
	int utenteRandom = rand()%100 + 1; //neste caso será um número ao calhas entre 1 e 100
	if(utenteRandom <= configs.prob_ute_ser_prio)//já está definido no configs
		tipoUtente = 2;  //cliente normal
	else
		tipoUtente = 1;  //o cliente prioritário
	
    //int tempoEsperaFila; tempo de espera na fila
	//char buffer[BUFFER_SIZE];
	pthread_mutex_lock(&mutex_posto_testagem); //############################################### FECHADO ###############################################
	idUtente = IdUtenteCount++;
	if(tipoUtente == 2) /// utente prioritario
		utentePriori++;
	else //if(tipoUtente == 1)-- utente normal
		utenteNormais++;
	pthread_mutex_unlock(&mutex_posto_testagem); //############################################# ABERTO #############################################
	
	
	///falta evento 2 cliente entra num centro de testagem<<<<<<<<
	
	
	tempoChegadaUtente = (int)time(0); //tempo que utente chega desde o "tempo zero"
	//neste caso escreve e envia o evento dois que é o cliente entra no posto de testagem
	sprintf(buffer, "%d  %d %d\n", (int)time(0), idUtente tipoUtente); //supostamente vai ser evento 3
	send(sockfd, buffer, sizeof(buffer), 0); //envia
	
	if(tipoUtente == 1) //utente normal
		sem_wait(&fila_prio_normal); //############################################### ESPERA ###############################################
	
	sem_wait(&fila_prio_normal); //############################################### ESPERA ###############################################
	
	//não é necessário proteger com um trinco porque está na tarefa posto de testagem, a postodetestagemDispo (postodetestagemDispo) só vai mudar depois do cliente
	mudar os dados do atendimento deste posto de testagem
	int pontodetestagemID = pontodetestagemDispo;
	
	//utente é retirado das filas
	//pthread_mutex_lock(&mutex_utente); //############################################### FECHADO ###############################################
	if(tipoutente == 2) //cliente é prioritario retirado da fila
		utentePriori--;
	else
		utenteNormais--; //cliente normal retirado da fila
	pthread_mutex_unlock(&mutex_cli); //############################################# ABERTO #############################################
	
	//cliente foi atendido
	tempoEsperaFila = (int)time(0) - tempoChegadaUtente;
	
	//utente é atendido no posto de testagem
	pontodetestagem[pontodetestagemID].tempDemoraTeste = tempoChegadaUtente + tempoEsperaFila; //tempo que demora a ser testado ou seja tempo que esperou na fila + tempo que demorou a chegar
	pontodetestagem[pontodetestagemID].IDutente = idUtente; //Associar um posto de testagem a um cliente
	if(tipoUtente == 1) 
	{/// utente normal
		UtenteNormais--;
		utentePontoTestagem--;
		utenteIsolamento++;
	}
	else
	{ //if(tipoCliente == 2 ou outros que neste caso não irá acontecer) cliente prioritario
		UtenteNormais--;
		utentePontoTestagem--;
		utenteIsolamento++;
	}

	//neste caso escreve e envia o evento 4, Utente é testado, sai do ponto de testagem e dirige-se ao isolamento
	
	aqui supostamente é preciso um trinco e nao um semaforo como esta no relatorio
	sprintf(buffer, "%d 4 %d %d\n", (int)time(0), idUtente, tipoUtente);
	send(sockfd, buffer, sizeof(buffer), 0);
	sem_wait(&pontodetestagem[pontodetestagemID].espera_ser_atendido); //############################################### POST ###############################################

	tempoChegadaUtenteIsolamento = (int)time(0); //tempo chegada utente ao isolamento

	if(tipoUtente == 1) //CLIENTE NORMAL
//aqui é trinco e nao semaforo
		sem_wait(&fila_normal_isolamento); //############################################### ESPERA ###############################################
	
	sem_wait(&fila_prio_isolamento); //############################################### ESPERA ###############################################
	int isolamentoID = isolamentoDispo; ///associar id ao isolamento
	if(tipoUtente == 2) 
		isolamento[isolamentoID].filaPrioisolamento++;
	else
		isolamento[isolamentoID].filaNormalIsolamento++;
	//utente é retirado da fila
	
	
	
	
	
	
	//utente é retirado da fila
	pthread_mutex_lock(&isolamento[isolamentoID].mutex_isolamento); //############################################### FECHADO ###############################################
	if(tipoUtente == 2) //utente é prioritario retirado do isolamento
		isolamento[isolamentoID].isolamento_prio--;
	else
		isolamento[isolamentoID].isolamento_normal--; //cliente normal retirado da fila
	pthread_mutex_unlock(&isolamento[isolamentoID].mutex_isolamento); //############################################# ABERTO #############################################
	
	//utente foi testado e econtra-se em isolamento
	a espera resultado do teste
	int tempoEsperaIsolamento = (int)time(0) - tempoChegadaUtenteIsolamento;
	isolamento[isolamentoID].tempoPassagem = tempoEsperaIsolamento + tempoChegadaUtenteIsolamento;
	isolamento[isolamentoID].internamento=isolamentoID;
	if(resultadoTeste==true)//Resultado positivo
	{
		internamento.[internamentoID].internamentoUtente
	}
	else
	{
		utenteCentroTestagem--;
	}
	isolamento[isolamentoID].IDUtente = idUtente; //Associar utente a isolamento
	//evento 4 ou seja cliente esta em isolamento
	sprintf(buffer, "%d 4 %d %d %d %d\n", (int)time(0), idUtente, tipoUtente, isolamento[isolamentoID].isolamentoID;
	//utente X prior/normal  esta no isolamento Y, no tempo X
	send(sockfd, buffer, sizeof(buffer), 0);
	utenteIsolamento-;
	utenteInternamento++;



}
	

FALTA EVENTOS!!!!!
	}

}



}
*/


void cria_evento(){


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

	

	//Receber mensagem do cliente
	while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
	{	

		int evento = atoi(client_message);

		switch(evento){
			case 1: {
				//Iniciar simulação
				//Função para gerar aleatoriamente um ou mais utentes com threads incluindo os atributos gerados pelo ficheiro de configuração
				//Função para gerar eventos
			}

			case 2: {
				//Continuar a simulacao 
			}
			case 3: {
				//Pausar a simulacao 
			}
			case 5: {
				//Terminar a simulacao
			}
		}


		//Enviar mensagem de volta ao cliente
		write(client_sock , client_message , strlen(client_message)+1);

		//----------------Comunicação-------------------
		
		

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
int main(int argc , char *argv[]) { 

	criasocket();

}
