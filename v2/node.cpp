/*
 * The aim of this node
 *
 * 
 * Created by: Matthew Cork (27-4-12)
 * 
 * Last Modified by: Matthew Cork (6-5-12)
 * 
 * Version 1.0
 * 
 */
 
#define IP "127.0.0.1"
#define PORT "6000"
#define NUMCON 1
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
 
 // internal functions
 void *get_in_addr(struct sockaddr *sa);

 
 
 
 int main(){
	int status, listenFd, sockFd;
	struct addrinfo hints, *p;
	struct addrinfo *servinfo; // results
	struct sockaddr_storage theirAddr; // connector's address information
	char connectedIP[INET_ADDRSTRLEN];
	socklen_t socketSize;
 
	memset(&hints,0,sizeof(hints));	// empty structure
	hints.ai_family = AF_UNSPEC	;	// IP unspecified
	hints.ai_socktype = SOCK_STREAM;	// SOCK_STREAM for TCP or SOCK_DGRAM for UDP
	hints.ai_flags = AI_PASSIVE	;	// Fills in IP Address 

	if ((status = getaddrinfo(NULL,PORT,&hints,&servinfo))!=0){
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
	printf("server: waiting for connections...\n");
	while(1){
		socketSize = sizeof theirAddr;
		sockFd = accept(listenFd, (struct sockaddr *)&theirAddr, &socketSize);
		if (sockFd == -1) {
			perror("accept");
			continue;
        }
		inet_ntop(theirAddr.ss_family,get_in_addr((struct sockaddr *)&theirAddr),connectedIP, sizeof connectedIP);
        printf("server: got connection from %s\n", connectedIP);
		
		//should do something more
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
