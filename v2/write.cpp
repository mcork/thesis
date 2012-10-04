#include "OG.h"
#include "libshmem.hpp"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>

using namespace std;

void writebuffer(char myFile[], char buffer[]);


int main(){
	shmem_region<OG> OGMap("OG");
	OG myOG;
	fstream myFile;
	int len;
	char buffer[140857600];
	
	myFile.open("1KB",ios::binary|ios::in);
	myFile.seekg(0, ios::end);
	len = myFile.tellg();
	myFile.seekg(0,ios::beg);
	myFile.read(&buffer, len);
	myFile.close();
	myOG.writeTime = time(0);
	myOG.myChange = true;
	writebuffer(myOG.myFile,buffer);
	OGMap.write(myOG);
	printf("Write Time %d\n",myOG.writeTime);
//	printf("Initial Values\nxVal - %d\n",myOG.xVal);
//	while (1){
		// Enter User Values
//		printf("Please Enter Xval - ");
//		scanf("%d",&myOG.xVal);
//		myOG.writeTime = time(0);
//		printf("Changed Values\nxVal - %d\n",myOG.xVal);
		// Write to shared memory
//		myOG.myChange = true;
//		OGMap.write(myOG);
//	}
	return 0;
}

void writebuffer(char myFile[], char buffer[]){
	for (int i=0;i<sizeof(buffer);i++){
		myFile[i]=buffer[i];
	}
}
