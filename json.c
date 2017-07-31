#include <stdio.h>
#include <string.h>
#include <stdlib.h>	// malloc()

#include "json.h"
#include "fct_utiles.h"

int scanForArrays(char* data);
int getObjectDataSize(char* data);

struct jsonObject* mkObject(char* jsonReceivedData);
struct jsonObject* mkArray(char* jsonReceivedData);


struct json* mkJsonData(char* jsonReceivedData)
{
	printf("Found %d arrays\n", scanForArrays(jsonReceivedData));
	printf("Main object is %d size\n", getObjectDataSize(jsonReceivedData));
	printf("Second object is %d size\n", getObjectDataSize(jsonReceivedData+20));
	printf(" : %s", jsonReceivedData+20);

	json* toReturn = (struct json*) malloc(sizeof(json));

	unsigned int i = 0;

	while(jsonReceivedData[i] != '{'){ i++; }
	toReturn->mainObject = mkObject(jsonReceivedData+i);
	
	return 0;
}


// For memory allocation of struct jsonObject*, must have the number of array to store
int scanForArrays(char* data)
{
	int dataArrayLevel = 0;	// When passing from 1 to 0 -> nArray ++
	int nArray = 0;

	for(size_t i = 0; i < strlen(data); i++)
	{
		if(data[i] == '['){ dataArrayLevel++; }
		if(data[i] == ']')
		{
			if(dataArrayLevel == 1){ nArray++; }
			dataArrayLevel--;
		}
	}

	return nArray;
}

int getObjectDataSize(char* data)
{
	int dataObjectLevel = 0;
	
	int initPos = 0;
	for(initPos = 0; data[initPos] != '{'; initPos++){}

	for(size_t i = 0; i < strlen(data); i++)
	{
		if(data[i] == '{'){ dataObjectLevel++; }
		if(data[i] == '}')
		{
			if(dataObjectLevel == 1){ return (int) (i - initPos); }
			dataObjectLevel--;
		}
	}
	return -1;
}

struct json* getJsonData(char* path)
{
	printf("Getting : %s\n", path);
	return 0;
}
