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
	myOG.yVal = 0;
	myOG.zVal = 0;
	myOG.myChange = true;
	OGMap.write(myOG);
	printf("Initial Values\nxVal - %d, yVal - %d, zVal - %d\n",myOG.xVal,myOG.yVal,myOG.zVal);
	while (1){
		// Enter User Values
		printf("Please Enter Xval - ");
		scanf("%d",&myOG.xVal);
		printf("Please Enter yVal - ");
		scanf("%d",&myOG.yVal);
		printf("Please Enter zVal - ");
		scanf("%d",&myOG.zVal);
		printf("Changed Values\nxVal - %d, yVal - %d, zVal - %d\n",myOG.xVal,myOG.yVal,myOG.zVal);
		// Write to shared memory
		myOG.myChange = true;
		OGMap.write(myOG);
	}
	return 0;
}
