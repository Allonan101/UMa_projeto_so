#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//Variaveis globais
int escolha_menu;
const char file_name[] = "Relatorio";


void criaficheiro(){
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


int main(void){ //menu de comandos
	
	printf("1 - Iniciar a simulacao \n"
		   "2 - Terminar a simulacao \n");
	
	scanf("%d",&escolha_menu);
	switch (escolha_menu) { 
		case 1: { 
			printf("\nIniciando a simulacao \n");
			criaficheiro();
			break; 
		} 
		case 2: { 
			printf("\nParando a simulacao \n");
			system("pause");
			return 1; 
		} 

		default: 
			main();
		} 

}





