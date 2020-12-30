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
sem_t fila_prio_normal;
sem_t isolamento_prio_normal;
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
	if(tipoUtentee == 2) /// clit prioritario
		utentePriori++;
	else //if(tipoUtente == 1)--clit normal
		utenteNormais++;
	pthread_mutex_unlock(&mutex_posto_testagem); //############################################# ABERTO #############################################


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