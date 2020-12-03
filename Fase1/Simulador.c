#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//Escrever para o ficheiro
int main(void) {
	//Variável do ficheiro
	int ficheiro;
	//variável de input de caracteres
	char input[50];
	//pede ao utilizador os caracteres
	printf("Insira uma frase:\n ");
	//guarda os valores na variavel os caracteres entre "" é para ler os espaços
	scanf("%[^\n]%*c", input);
	//abre o ficheiro ou cria caso este não exista
	ficheiro = open ("Relatorio", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	//escreve no ficheiro os carateres escritos pelo utilizador
	write (ficheiro,input, 50);
	//fecha o ficheiro
	close(ficheiro);
}