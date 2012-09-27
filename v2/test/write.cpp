#include "random.h"
#include "libshmem.hpp"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

int main(){
	shmem_region<OG> OGMap("OG");
	OG myOG;
	int update;
	myOG.value = 0;
	OGMap.write(myOG);
	printf("Initial Values\nxVal - %d, yVal - %d, zVal - %d\n",myOG.xVal,myOG.yVal,myOG.zVal);
	while (1){
		// Enter User Values
		update = fgetc(stdin);
		myOG.value ++;
		OGMap.write(myOG);
	}
	return 0;
}
