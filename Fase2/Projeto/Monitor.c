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
		printf("Erro no acesso ao ficheiro\n");
		exit(0);
	}
}

void client_socket(){
	int sock;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];
	
	//Criar um socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Nao foi possivel criar o socket");
	}
	puts("Socket criado");
	
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

	
	//Loop de comunicação com o simulador
	while(1)
	{

		printf("1 - Iniciar a simulacao \n"
		"2 - Continuar a simulacao \n"
		"3 - Pausar a simulacao \n"
		"4 - Imprimir os resultados \n"
		"5 - Terminar a simulacao \n");

		scanf("%s" , message);

		int x = atoi(message);
		
		menu(x);

		//Envio de dados
		if( send(sock , message , strlen(message) , 0) < 0)
		{
			puts("Falha ao enviar");
			return 1;
		}
		
		//Resposta do simulador
		if( recv(sock , server_reply , 2000 , 0) < 0)
		{
			puts("recv falhou");
			break;
		}
		
		puts("Resposta do simulador :");
		puts(server_reply);
	}
	
	close(sock);
	return 0;
}

void menu(x){
	printf("%d",x);
	switch (x) { 
		case 1: { 
			printf("\nIniciando a simulacao \n");
			break;
		} 

		case 2: { 
			printf("\nContinuando a simulacao \n");
			
		} 
		case 3: { 
			printf("\nPausando a simulacao \n");

		} 
		case 4: { 
			printf("\nImprimindo o resultados \n");
			lerficheiro();

		} 
		case 5: { 
			printf("\nTerminando a simulacao \n");
	
		}
	} 
}

int main(int argc , char *argv[]){ 
	client_socket();
}





