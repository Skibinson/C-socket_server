#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
//#include <fcntl.h>

#define PORT 10213
#define LOG "msfile.log"

int stringlen(char *str) 
{
    int i=0;
    while (str[i] > 0 && str[i] <= 127 ) {
        i++;
    } 
    return i;
}

void strfree(char *string)
{
	int i;
	i = 0;
	while (string[i] != '\0') {
		string[i] = '\0';
		i++;
	}
}

int main(void) {
	FILE *logfile;
	time_t t;
	pid_t childpid; 
	struct sockaddr_in stSockAddr;
	int i32SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	socklen_t addrlen = sizeof(stSockAddr);
    char str[255];
	char author[] = " Сервер написан Скибиным А.С. М3О-109БВ-24\0";
	char *outstring;
	char *strptr;
	int i, j, tmp, l;


	if (i32SocketFD == -1) {
		perror("Socket error");
		exit(EXIT_FAILURE);
	}

	memset(&stSockAddr, 0, sizeof (stSockAddr));

	stSockAddr.sin_family = PF_INET;
	stSockAddr.sin_port = htons(PORT);
	stSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(i32SocketFD, (struct sockaddr*) &stSockAddr, sizeof (stSockAddr)) == -1) {
		perror("Bind error");

		close(i32SocketFD);
		exit(EXIT_FAILURE);
	}

	if (listen(i32SocketFD, 10) == -1) {
		perror("Listen error");

		close(i32SocketFD);
		exit(EXIT_FAILURE);
	}	
	

	logfile = fopen(LOG, "a");
	time(&t);
	fprintf(logfile, "Server UP: %s", ctime(&t));

	for (;;) {
		int i32ConnectFD = accept(i32SocketFD, (struct sockaddr*) &stSockAddr, &addrlen);

		if (i32ConnectFD < 0) {
			perror("Accept error");
			close(i32SocketFD);
			exit(EXIT_FAILURE);
		}
		time(&t);
		fprintf(logfile, "Client connection: %s", ctime(&t));
		
		if ((childpid = fork()) == 0) {
			read(i32ConnectFD, str, sizeof(str));
			time(&t);
			fprintf(logfile, "Got message: '%s' %s", str, ctime(&t));
			l = stringlen(str);
			for (i = 0, j = l - 1; i < j; i++, j--) {
				tmp = str[i];
				str[i] = str[j];
				str[j] = tmp;
			}
		
			outstring = strcat(str, author);

			time(&t);
			fprintf(logfile, "Send message: '%s' %s", str, ctime(&t));

			write(i32ConnectFD, outstring, l+sizeof(author));
			shutdown(i32ConnectFD, SHUT_RDWR);

			close(i32ConnectFD);
			time(&t);
			fprintf(logfile, "Client disconnected: %s\n", ctime(&t));

			strptr = &str[0]; 
			strfree(strptr);

			fflush(logfile);
		}
	}
	

	return 0;
}
