/*
 *  Node program that allows the sharing data over network eiher via TCP or UDP
 * 
 *  Created by Matthew Cork 12/03/12
 * 
 */

using namespace std;
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ServerSocket.h>
#include "node.h"

#define STARTPORT 6000
#define CHANNELRANGE 10

#define ENDIP "127.0.0.1"

int main(int argc, char **argv){

	try{
		ServerSocket server(STARTPORT);
		
		while (true){
			ServerSocket new_sock;
			server.accept (new_sock);
			try{
				while(true){
					new_sock >> data;
					new_sock << data;
				}
			} catch (SocketException& e){}
		}
	} catch (SocketException& e){
		cout << "Exception was caught:" <<e.description() << "\nExit";
	}
	return 0; 
}
