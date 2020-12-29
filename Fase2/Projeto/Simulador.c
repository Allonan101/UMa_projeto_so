#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>	
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>	


void load_conf_simulador(){ //Load do ficheiro de configuracao_simul.conf para listas
	
}

void cria_evento(){


}
void escreve_relatorio(char* input){ //Escrever para o ficheiro

	FILE *ficheiro = fopen("Relatorio.txt", "a");

	if (ficheiro == NULL){
		printf("Erro ao abrir ficheiro\n");
		exit(-1);
	}

	fprintf(ficheiro, "%s\n", input);
	fclose(ficheiro);

}

void criasocket(){
	
	remove ("Relatorio.txt");

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
		escreve_relatorio(client_message);

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
		write(client_sock , client_message , strlen(client_message));

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