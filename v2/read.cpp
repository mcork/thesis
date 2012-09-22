#include "OG.h"
#include "libshmem.hpp"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

int main(){
	shmem_region<OG> OGMap("OG");
	OG* myOG;
	char update;
	int EntryValues[2];
	
	while (1){
		
		OGMap.status(EntryValues);
		printf("Read Entry: %d, Write Entry: %d\n",EntryValues[0],EntryValues[1]);
		
		myOG = OGMap.read();
		update = fgetc(stdin);
		printf("New OG Xval is %d\n",myOG->xVal);
		printf("New OG Yval is %d\n",myOG->yVal);
		printf("New OG Zval is %d\n",myOG->zVal);
	}

	return 0;
}
