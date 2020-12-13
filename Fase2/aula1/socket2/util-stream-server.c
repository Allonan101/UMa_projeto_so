#define MAXLINE 512

/* Servidor do tipo socket stream.
   Manda linhas recebidas de volta para o cliente */

str_echo(sockfd)
int sockfd;
{
	int n, i, inc;
	char line[MAXLINE];

	for (;;) {

		/* Le a primeira linha do socket: o incremento */
		n = readline(sockfd, line, MAXLINE);
		if (n == 0)
			return;
		else if (n < 0)
			err_dump("str_echo: readline error");

		line[n-1] = '\0';
		inc = atoi(line);

		/* Le a segunda linha do socket: os caracteres */
		n = readline(sockfd, line, MAXLINE);
		if (n == 0)
			return;
		else if (n < 0)
			err_dump("str_echo: readline error");

		for(i=0; i<n-1; i++) {
		  line[i] += inc;
		}
		/* Manda linha de volta para o socket. n conta com
		   o \0 da string, caso contrßrio perdia-se sempre 
		   um caracter! */
		if (writen(sockfd, line, n) != n)
			err_dump("str_echo: writen error");
	}
}
