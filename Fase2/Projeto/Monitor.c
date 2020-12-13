#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr


//Variaveis globais
int escolha_menu;
const char file_name[] = "Relatorio.txt";


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




void menu(&escolha_menu){

	printf("1 - Iniciar a simulacao \n"
		"2 - Continuar a simulacao \n"
		"3 - Pausar a simulacao \n"
		"4 - Imprimir os resultados \n"
		"5 - Terminar a simulacao \n");
	
	scanf("%d",escolha_menu);
	switch (escolha_menu) { 
		case 1: { 
			printf("\nIniciando a simulacao \n");
			client_socket();
			break; 
		} 

		case 2: { 
			printf("\nContinuando a simulacao \n");
			system("pause");
			break; 
		} 
		case 3: { 
			printf("\nPausando a simulacao \n");
			system("pause");
			return 1; 
		} 
		case 4: { 
			printf("\nImprimindo o resultados \n");
			system("pause");
			return 1; 
		} 
		case 5: { 
			printf("\nTerminando a simulacao \n");
			system("pause");
			return 1; 
		} 

		default: 
			menu(escolha_menu);
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

		printf("Enter escolha : ");
		scanf("%s" , &message);
		
		if(menu(&message)) == 1){
			continue;
		}
		else(){
			break;
		}

		//Envio de dados
		if( send(sock , message , strlen(message) , 0) < 0)
		{
			puts("Falha ao enviar");
			return 1;
		}
		
		//Resposta do simulador
		if( recv(sock , server_reply , 2000 , 0) < 0)
		{
			puts("recv failed");
			break;
		}
		
		puts("Server reply :");
		puts(server_reply);
	}
	
	close(sock);
	return 0;
}

int main(void){ //menu de comandos
	
	menu(&escolha_menu);

}





