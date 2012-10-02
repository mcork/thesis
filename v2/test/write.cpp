#include "OG.h"
#include "libshmem.hpp"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

void fillArray(int* array, int count);
void displayArray(int* array);

int main(){
	shmem_region<OG> OGMap("OG");
	OG myOG;
	int count=0;
	OGMap.write(myOG);
	while (1){
		// Enter User Values
		fillArray(myOG.value, count);
		cout << "--- DATA GOING IN --- count = " << count << endl;
		displayArray(myOG.value);
		// Write to shared memory
		OGMap.write(myOG);
		count++;
		sleep(1);
	}
	return 0;
}

void displayArray(int* array){
	for (int i=0; i<OGSIZE; i++){
		cout << array[i];
	}
	cout << endl;
}

void fillArray(int* array, int count){
	for (int i=0; i<OGSIZE; i++){
		array[i]= count;
	}
}
