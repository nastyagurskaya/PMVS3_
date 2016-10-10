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
{	printf("I am a client and i am finish!\n");
	close(sockfd);
	return 0;
}
