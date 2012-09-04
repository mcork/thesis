/*
 * The aim of this node is to communicate the occupancy grid over an adhoc network
 *
 * Created by: Matthew Cork (27-3-12)
 *
 * Last Modified by: Matthew Cork (31-8-12)
 *
 * Version 2.4
 *
 */

#define SERVERPORT 6000
#define SERVER "6000"
#define NUMCON 1
#define DEBUGMODE 1	// shows error messages on stdout to allow for debugging
#define SENDFILE "TODO"
#define IPADDR "127.0.0.1"	// can't use loopback anymore as clases will happen with node recv and send

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <signal.h>
#include "libshmem.hpp"
#include "OG.h"

using namespace std;
// internal functions
void *get_in_addr(struct sockaddr *saq);
int SendMessage(void *message, int messageSize, int socketFd);

 int main(){
	int status, listenFd, ServerSockFd;
	int sendCheck, readCheck;		// check to for internal functions
	struct addrinfo hints, *p;
	struct addrinfo *servinfo; // results
	struct sockaddr_storage theirAddr; // connector's address information
	char connectedIP[INET_ADDRSTRLEN];
	socklen_t socketSize;
// stuff needed for recv connection to other connection
	struct sockaddr_in recvDest;
	int recvSock;
	// shared memory
	shmem_region<OG> OGMap("OG");
	OG myOG;
	OG messageRecv;

/////////////////////////////////////////////////////////////////////////////
// stuff that is testing
	char *message = "this is my message to send";
	//char messageRecv[1000]=""; 				// message from connection
	fstream mySendingData;

/////////////////////////////////////////////////////////////////////////////
	// socket details for read
	recvSock = socket(AF_INET, SOCK_STREAM,0);
	memset(&recvDest, 0, sizeof(recvDest));
	recvDest.sin_family = AF_INET;
	recvDest.sin_addr.s_addr = inet_addr(IPADDR);
	recvDest.sin_port = htons(SERVERPORT);

	// sockets for send port

	memset(&hints,0,sizeof(hints));	// empty structure
	hints.ai_family = AF_UNSPEC	;	// IP unspecified
	hints.ai_socktype = SOCK_STREAM;	// SOCK_STREAM for TCP or SOCK_DGRAM for UDP
	hints.ai_flags = AI_PASSIVE	;	// Fills in IP Address

// initialising shared memory receptacle
	myOG.xVal=0;
	myOG.yVal=0;
	myOG.zVal=0;
	myOG.myChange=false;
	myOG.otherChange=false;

// initialising recv connection
	connect(recvSock, (struct sockaddr *)&recvDest, sizeof(struct sockaddr));

// Initialising server connection
	if ((status = getaddrinfo(NULL,SERVER,&hints,&servinfo))!=0){
		fprintf(stderr,"get addrinfo error: %s\n", gai_strerror(status));
	}

	// error checking to ensure server binds
	for (p = servinfo; p != NULL;p=p->ai_next){
		// checking socket exists
		if ((listenFd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
	            perror("server: socket");
        	    continue;
       		}
	        // bind socket and check for errors
        	if (bind(listenFd, p->ai_addr, p->ai_addrlen) == -1) {
        	    perror("server: bind");
        	    continue;
        	}
        	break;
	}
	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}
	if (listen(listenFd, NUMCON) == -1) {
		perror("listen");
		return 1;
   	}
	// Server Info
	if (DEBUGMODE == 1)printf("server: waiting for connections...\n");
	while(1){
		socketSize = sizeof theirAddr;
		ServerSockFd = accept(listenFd, (struct sockaddr *)&theirAddr, &socketSize);
		if (ServerSockFd == -1) {
			perror("accept");
			continue;
	        }
		inet_ntop(theirAddr.ss_family,get_in_addr((struct sockaddr *)&theirAddr),connectedIP, sizeof connectedIP);
		if (DEBUGMODE == 1)printf("server: got connection from %s on %d\n", connectedIP,ServerSockFd);
		while(1){
			// Broadcast Alive Message

	/**************** READ INCOMING MESSAGES FROM OTHERS *********************************/
			// Need to read data in and deal with it
			readCheck=recv(recvSock, &messageRecv,sizeof(messageRecv),MSG_DONTWAIT);
			if (DEBUGMODE == 1)printf("Read Check: %d\n",readCheck);
			if (readCheck > -1) {
				printf("Got data\n");
				messageRecv.xVal = myOG.xVal;
				messageRecv.yVal = myOG.yVal;
				messageRecv.zVal = myOG.zVal;
				myOG.otherChange = true;
				OGMap.write(myOG);
			}
	/***************** SYNC OG WITH LOCAL  ****************************************************/
			// check to see if shared memory has something to send
			myOG = *OGMap.read();
			if (myOG.myChange == true){
	/*************************************************************************************/
	/*************** TEMP: OPEN FILE FROM FS ***********************************************
			ifstream::pos_type size;
			char * memblock;
			if (DEBUGMODE == 1)printf("Starting Open Files\n");

			mySendingData.open(SENDFILE,ios::binary|ios::out|ios::in);
			if (mySendingData.is_open()){
				mySendingData.seekg(0,ios::end);		// File Pointer at end of File
				int fileSize = mySendingData.tellg();		// determining file size
				mySendingData.seekg(0,ios::beg);		// File pointer at beginning of file
				memblock = new char [fileSize];
				mySendingData.read(memblock, fileSize);		// loading file into memblock
				if (DEBUGMODE == 1)printf("Got past Reading\n");
				sendCheck = SendMessage(memblock,fileSize,ServerSockFd);
				if (sendCheck != 0){
					if (DEBUGMODE == 1)printf("Sending Failed: %d\n",sendCheck);
				}
				if (DEBUGMODE == 1)printf("Got past Sending\n");

				mySendingData.close();
			}else{
				if (DEBUGMODE == 1)printf("Failed to Open File\n");
			}
	/*******************************************************************************************/
	/*************** SEND MESSAGE TO OTHERS ****************************************************/

				if  (DEBUGMODE == 1)printf("Starting Send Sequence\n");
				sendCheck=SendMessage(&myOG,sizeof(myOG),ServerSockFd);
				if (sendCheck != 0){
					fprintf(stderr,"SENDING ERROR:Failure To Send\n");
					if  (DEBUGMODE == 1)fprintf(stdout,"SENDING ERROR:Failure To Send - %d\n",sendCheck);
					break;
				}
			}
	/*******************************************************************************************/
		}
	}
	// shouldn't ever reach here but just in case free up the relevant memory
	freeaddrinfo(servinfo);	// free memory
	return 0;

 }


 // get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// sends the whole message
int SendMessage(void *message, int messageSize, int socketFd){
	int bytesSent=0;
	int *messagePtr=(int*)message;
	// Loop through all of these steps until all sent
	signal(SIGPIPE, SIG_IGN); // stopping server crashing with disconnected client

	while (messageSize > 0){
		// send all bytes
			bytesSent = write(socketFd, messagePtr, messageSize);
		// check how many bytes were actually sent and handle errors if zero
		if (bytesSent < 0){
			//Error Handle
			if (DEBUGMODE == 1)printf("Error Writing to socket\n");
			return -1;
		}
		// determine how many bytes left to be written
		messageSize = messageSize - bytesSent;
		// advance buffer point by that amount
		messagePtr = messagePtr + bytesSent;
		if  (DEBUGMODE == 1)printf("message Size - %d, bytes sent - %d\n",messageSize,bytesSent);

	}
	if  (DEBUGMODE == 1)printf("Message Finishing Sending\n");
	return 0;
}

