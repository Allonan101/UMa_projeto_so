#include <stdio.h>
#define MAXLINE 512

/* Cliente do tipo socket stream.
   Le string de fp e envia para sockfd.
   Le string de sockfd e envia para stdout */

str_cli(fp, sockfd)
FILE *fp;
int sockfd;
{
	int n;
	char sendline[MAXLINE], recvline[MAXLINE+1];

	while (fgets(sendline, MAXLINE, fp) != NULL) {
		
            /* Envia string para sockfd. Note-se que o \0 nao 
               e enviado */
		n = strlen(sendline);
		if (writen(sockfd, sendline, n) != n)
			err_dump("str_cli: writen error on socket");
		fgets (sendline, MAXLINE, fp);
		n = strlen(sendline);
		if (writen(sockfd, sendline, n) != n)
			err_dump("str_cli: writen error on socket");

		/* Tenta ler string de sockfd. Note-se que tem de 
		   terminar a string com \0 */

		n = readline(sockfd, recvline, MAXLINE);
		if (n<0)
			err_dump("str_cli:readline error");
		recvline[n] = 0;

		/* Envia a string para stdout */

		fputs(recvline, stdout);
	}
	if (ferror(fp))
		err_dump("str_cli: error reading file");
}
