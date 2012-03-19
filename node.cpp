/*
 *  Node program that allows the sharing data over network eiher via TCP or UDP
 * 
 *  Created by Matthew Cork 12/03/12
 * 
 */

using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BASEPORT 6000
#define CHANNELRANGE 10

#define MYIP "127.0.0.1"
#define MAXNUMMAVS 15

// Internal Functions
void closeAllConnections(int SocketIDs[MAXNUMMAVS]);
void serverInit(struct sockaddr_in serv);
void closeConnection(int SocketID);

int main(int argc, char **argv){

	char msg[] = "Connecting to Localhost";
	
	struct sockaddr_in dest; 							// Remote machine details
	struct sockaddr_in serv; 							// Server Details
	int mysocket;										// Incoming connection Socket
	unsigned int socketSize = sizeof(struct sockaddr_in);	
	
	// Setting up server details
	serverInit(serv);

	mysocket = socket(AF_INET, SOCK_STREAM, 0);
	
	//Bind server to socket
	bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr));
	
	// Start server listening
	listen(mysocket,1);
	int consocket = accept(mysocket, (struct sockaddr *)&dest, &socketSize);

	printf("\n\n\nconsocket %d\n\n\n",consocket);
	
	while (consocket)
	{
		printf("Incoming connection from %s - sending welcome\n", inet_ntoa(dest.sin_addr));
		send(consocket, msg, strlen(msg), 0);
	}
	
	close(consocket);
	close(mysocket);
	return EXIT_SUCCESS;

	return 0; 
}
	
void closeConnection(int SocketID){
	close(SocketID);
}
	
	
void closeAllConnections(int SocketIDs[MAXNUMMAVS]){
	int index =0;
	while (index < MAXNUMMAVS){
		close(SocketIDs[index]);
		index ++;
	}
}

void serverInit (struct sockaddr_in serv){
	memset (&serv , 0, sizeof(serv));
	serv.sin_family = AF_INET;							// set type of IPV4 or IPV6
	serv.sin_addr.s_addr = inet_addr(MYIP);		// set ip address see below 
	/*
	 *    inet_addr("XXX.XXX.XXX.XXX") - for actual address
	 * 	  INADDR_ANV - for any interface
	 *
	 */
	 serv.sin_port = htons(BASEPORT);	
}
