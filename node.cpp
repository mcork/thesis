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
#define TERM "\\EnD\\"

// Internal Functions
void closeAllConnections(int SocketIDs[MAXNUMMAVS]);
int serverInit(struct sockaddr_in serv, int mySocket);
void closeConnection(int SocketID);

int main(int argc, char **argv){

	char msg[]= "Connecting to Localhost";
	
	char msg[strlen(amsg)+5];
	strcpy(msg,amsg);
	strcat(msg,TERM);
	
	
	struct sockaddr_in dest; 							// Remote machine details
	struct sockaddr_in serv; 							// Server Details
	int mySocket=0;										// Incoming connection Socket
	unsigned int socketSize = sizeof(struct sockaddr_in);	
	
	// Setting up server details
	mySocket = serverInit(serv, mySocket);
	
	// Start server listening
	listen(mySocket,1);
	int consocket = accept(mySocket, (struct sockaddr *)&dest, &socketSize);
		
	while (consocket)
	{
		printf("Incoming connection from %s - sending msg = %s\n", inet_ntoa(dest.sin_addr), msg);
		printf("consocket %d\n",consocket);
			printf("msg[] - %s\n",msg);

		send(consocket, msg, strlen(msg), 0);
	}
	
	closeConnection(consocket);
	closeConnection(mySocket);
	return EXIT_SUCCESS;
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

int serverInit (struct sockaddr_in serv, int mySocket){
	memset (&serv , 0, sizeof(serv));
	serv.sin_family = AF_INET;							// set type of IPV4 or IPV6
	serv.sin_addr.s_addr = inet_addr(MYIP);		// set ip address see below 
	/*
	 *    inet_addr("XXX.XXX.XXX.XXX") - for actual address
	 * 	  INADDR_ANY - for any interface
	 *
	 */
	serv.sin_port = htons(BASEPORT);
	 
	mySocket = socket(AF_INET, SOCK_STREAM, 0);			// setup socket
	
	//Bind server to mySocket
	bind(mySocket, (struct sockaddr *)&serv, sizeof(struct sockaddr));
	return mySocket;
}
