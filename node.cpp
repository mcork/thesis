/*
 *  Node program that allows the sharing data over network eiher via TCP or UDP
 * 
 *  Created by Matthew Cork 12/03/12
 * 
 */

using namespace std;
#include  <sys/types.h>
#include <sys/socket.h>
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>


#DEFINE STARTPORT 6000
#DEFINE CHANNELRANGE 10

#DEFINE ENDIP "127.0.0.1"

  
