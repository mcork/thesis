// Version 2.1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fstream>
#include <iostream>

#define MAXRCVLEN 1048576
#define PORTNUM 6000
#define DEBUGMODE 1 //allows for debugging mode

using namespace std;

int parseMessage(int length, char buffer[]);

int main  (int argc, char *argv[]){
	char buffer[MAXRCVLEN +1];
	int len=-1, mysocket, loopCount=0;
	int recvLen=0;
	struct sockaddr_in dest;

	fstream myFile;
	myFile.open("transfer.txt",ios::binary|ios::out);

	mysocket = socket(AF_INET, SOCK_STREAM,0);

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr("10.0.0.2");
	dest.sin_port = htons(PORTNUM);

	connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	while (len !=0){
		len = recv(mysocket, buffer, MAXRCVLEN, 0);
		printf("Loop Counter (%d)\nAlso len(%d)\nRecvLen (%d)\n",loopCount,len,recvLen);
		recvLen += len;
		loopCount++;
		if (len < 1448){
			len=0;
		}
	}
	printf("Finished recieving. Total Length %d\n",recvLen);
//	len = parseMessage(len, buffer);

	if (myFile.is_open()){
		if (DEBUGMODE == 1)printf("File Should Be Open\n");
		myFile.seekg(0, ios::beg);
		myFile.write(buffer, recvLen);
		myFile.close();
	}else{
		if (DEBUGMODE == 1)printf("Failed to Open File\n");		
	}

	printf("Received (%d bytes). Size of buffer %d.\n \%s", recvLen, sizeof(buffer), buffer);
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
