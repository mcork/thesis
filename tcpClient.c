

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAXRCVLEN 500
#define PORTNUM 6000

int main  (int argc, char *argv[]){
	char buffer[MAXRCVLEN +1];
	int len, mysocket;
	struct sockaddr_in dest;
	
	mysocket = socket(AF_INET, SOCK_STREAM,0);
	
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(PORTNUM);
	
	connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	
	len = recv(mysocket, buffer, MAXRCVLEN, 0);
	buffer[len] = '\0';
	
	printf("Received %s (%d bytes).\n",buffer, len);
	close(mysocket);
	return EXIT_SUCCESS;
}
