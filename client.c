#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

int main(int argc, char* argv[])
{	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];
	portno = 448;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("ERROR opening socket");
		exit(0);
	}
	server = (struct hostent *)gethostbyname("127.0.0.1");
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((const char *)server->h_addr, 
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
		perror("ERROR connecting\n");
		exit(0);
	}
	printf("Please enter command: ");
	bzero(buffer,256);
	fgets(buffer,255,stdin);
	n = write(sockfd,buffer,strlen(buffer));
	if (n < 0) {
		perror("ERROR writing to socket\n");
		exit(0);
	}
	bzero(buffer,256);
	while (1) {
		n = read(sockfd,buffer,255);
		if (n < 0){
			perror("ERROR reading from socket\n");
			exit(0);
		}
		if (strlen(buffer)==1&&!strcmp(buffer,"N")) {
			break;
		}
		printf("%s",buffer);
		bzero(buffer,256);
	}
	printf("I am a client and i am finish!\n");
	close(sockfd);
	return 0;
}
