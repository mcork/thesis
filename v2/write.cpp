#include "OG.h"
#include "libshmem.hpp"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

int main(){
	shmem_region<OG> OGMap("OG");
	OG myOG;
	myOG.xVal = 0;
	myOG.writeTime = time(0);
	myOG.myChange = true;
	OGMap.write(myOG);
	printf("Initial Values\nxVal - %d\n",myOG.xVal);
	while (1){
		// Enter User Values
		printf("Please Enter Xval - ");
		scanf("%d",&myOG.xVal);
		myOG.writeTime = time(0);
		printf("Changed Values\nxVal - %d\n",myOG.xVal);
		// Write to shared memory
		myOG.myChange = true;
		OGMap.write(myOG);
	}
	return 0;
}
