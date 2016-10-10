#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

//вариант 2 - передать пронрамму popen

typedef struct _params{
	int socket;
}params;
void *compute(void* arg){
	char buffer[256];
	int n;
	bzero(buffer,256);
	params parameter = *( (params*) arg );
	n = read(parameter.socket,buffer,255);
	if (n < 0) {
		perror("ERROR reading from socket");
	}
	else
	{
		FILE *fp;
		int status;
		char path[255];
		fp = popen(buffer, "r");
		if (fp == NULL) {
			perror("ERROR executing");
		}
		bzero(path,256);
		while (fgets(path, 100, fp) != NULL) {
			n = write(parameter.socket,path,strlen(path));
			if (n < 0) {
				perror("ERROR writing");
			}
			bzero(path,256);
		}
		strcpy(path, "N");
		n = write(parameter.socket, path, strlen(path));
		if (n<0) {
			perror("ERROR writing");
		}
	}
	close(parameter.socket);
}

int main()
{
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("ERROR opening socket");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 448;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		 perror("ERROR on binding");
		 exit(0);
	}
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	while (1) {
		int new_socket=0;
		if ((new_socket = accept(sockfd, 
			(struct sockaddr *) &cli_addr, &clilen))<0) {
				perror("accept");
				exit(EXIT_FAILURE);
		}
		else {
			params parameter;
			parameter.socket = new_socket;
		#ifdef PROCESS
			pid_t childId = 0;
			if ((childId=fork())==-1) {
				perror("fork");
				exit(1);
			}
			if (childId == 0) {
				printf("Started processing request\n");
				compute(&parameter);
				printf("Finished processing request\n");
				return 0;
			}
		#else
			printf("Started processing request");
			pthread_t thread1;
			int result = pthread_create(&thread1, NULL, compute, &parameter);
			if (result != 0) {
				perror("Error creating thread");
			}
		#endif
		}
	}
	close(sockfd);
	return 0;
}
