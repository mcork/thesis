

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

int parseMessage(int length, char buffer[]);

int main  (int argc, char *argv[]){
	char buffer[MAXRCVLEN +1];
	int len=1, mysocket;
	struct sockaddr_in dest;
	
	mysocket = socket(AF_INET, SOCK_STREAM,0);
	
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr("192.168.91.130");
	dest.sin_port = htons(PORTNUM);

	connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	while (len > 0){

		len = recv(mysocket, buffer, MAXRCVLEN, 0);

		//len = parseMessage(len, buffer);


		printf("Received \"%s\" (%d bytes).\n",buffer, len);
	}
	close(mysocket);
	return EXIT_SUCCESS;
}

int parseMessage(int len, char buffer[]){
	int i, total;
	total = len;
	for (i=0;i<=len;i++){
		if ((buffer[i] == '\\')&&(buffer[i+1] == 'E')&&(buffer[i+2] == 'n')&&(buffer[i+3] == 'D')&&(buffer[i+4] == '\\')){
			printf("found pattern\n");
			len = i;
		}
	}
	i--;
	while(i<=total){
		buffer[i]='\0';				// zero out all data from the end of message character
		i++;
	}
	return len;
}
