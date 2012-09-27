#include "random.h"
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
	int EntryValues[3];

	while (1){
		OGMap.status(EntryValues);
		update = fgetc(stdin);
		printf("Checksum: %d, Read Entry: %d, Write Entry: %d\n",EntryValues[0],EntryValues[1],EntryValues[2]);
		myOG = OGMap.read();
		if ((int*)myOG == (int*)-1) {
			printf("No Data has been written \n");
		}else{
			std::cout << "--- Data ---" << endl << *OG.value <<endl;
		}
	}

	return 0;
}
