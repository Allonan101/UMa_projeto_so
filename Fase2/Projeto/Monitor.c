#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>	//strlen
#include <stdlib.h>	//strlen
#include <sys/socket.h>
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>	//write
#include <pthread.h> //for threading , link with lpthread


//Variaveis globais
const char file_name[] = "Relatorio.txt";
int estado_centro = 0; //Fechado = 0/Aberto = 1
int iniciado = 0; 

void load_conf_monitor(){
	
}

void lerficheiro(){ 
	int ficheiro;
	char output[50];
	ficheiro = open(file_name, O_RDONLY);

	if(access(file_name, F_OK ) != -1){ //verifica o acesso ao ficheiro
		printf("Lendo o ficheiro: \n");
		read(ficheiro,output,50);
		close(ficheiro);
		printf(output);
		printf("\n");
	}
	else{
		printf("Erro no acesso ao ficheiro \n");
		exit(0);
	}
}

void escreve_relatorio(char* input){ //Escrever para o ficheiro

	FILE *ficheiro = fopen("Relatorio.txt", "a");

	if (ficheiro == NULL){
		printf("Erro ao abrir ficheiro \n");
		exit(-1);
	}

	fprintf(ficheiro, "%s\n", input);
	fclose(ficheiro);

}

void menu(x){

	switch (x) { 
		case 1: { 
			printf("Iniciando a simulacao \n");
			break;
		} 

		case 2: { 
			printf("Continuando a simulacao \n");
			break;
		} 
		case 3: { 
			printf("Pausando a simulacao \n");
			break;
		} 
		case 4: { 
			printf("Imprimindo o resultados \n");
			lerficheiro();
			break;

		} 
		case 5: { 
			printf("####Centro de testagem fecha####\n");
			estado_centro = 0;
			break;
		}
	} 
}

void client_socket(){
	int sock;
	struct sockaddr_in server;
	char message[1000] , resposta_server[2000];
	
	//Criar um socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Nao foi possivel criar o socket");
	}
	puts("Socket criado \n");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );

	//Conectar ao simulador
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("Falha ao conectar socket.");
		return 1;
	}
	
	puts("Conectado\n");
	puts("Apagando o relatorio anterior...\n");
	remove("Relatorio.txt");

	char mensagem_de_envio[20] = "Enviado:";
	char mensagem_do_simulador[20] = "Recebido:";
	
	//Loop de comunicação com o simulador
	while(1)
	{

		printf("1 - Iniciar a simulacao \n"
		"2 - Continuar a simulacao \n"
		"3 - Pausar a simulacao \n"
		"4 - Imprimir os resultados \n"
		"5 - Terminar a simulacao \n\n");

		scanf("%s" , message);

		escreve_relatorio(mensagem_de_envio);
		escreve_relatorio(message);

		int evento_envio = atoi(message); //Evento a enviar

		if(evento_envio == 1 && iniciado == 1){
			evento_envio = 2;
			printf("Simulacao já iniciada!\n");
		}
		if(evento_envio == 1 && iniciado == 0){
			iniciado = 1;
		}

		menu(evento_envio);

		//Envio de dados
		if( send(sock , message , strlen(message)+1 , 0) < 0)
		{
			puts("Falha ao enviar");
			return 1;
		}
		
		if(evento_envio == 5){
			printf("Terminando com o codigo: %d \n",evento_envio);
			exit(0);
			break;
		}

		//Resposta do simulador
		if( recv(sock , resposta_server , 2000 , 0) < 0)
		{
			puts("recv falhou");
			break;
		}
		
		//Eventos recebidos
		int evento_recebido = atoi(resposta_server);

		if(evento_recebido==1 && estado_centro ==0){
			printf("\n#####Centro de testagem abre#####\n");
		 	estado_centro = 1;	
		}
		if(evento_recebido==2){
		
		}
		if(evento_recebido==3){
		
		}
		if(evento_recebido==4){
		
		}
		if(evento_recebido==5){
		
		}
		if(evento_recebido==6){
		
		}
		if(evento_recebido==7){
		
		}
		if(evento_recebido==8){
		
		}
		if(evento_recebido==9){
		
		}

		escreve_relatorio(mensagem_do_simulador);
		escreve_relatorio(resposta_server);

		printf("Evento recebido do simulador: %d",evento_recebido);
		printf("\n********************************\n");
	}
	
	close(sock);
	return 0;
}


int main(int argc , char *argv[]){ 
	client_socket();
}





