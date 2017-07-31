#include <stdio.h>
#include <string.h>
#include <stdlib.h>	// malloc()
#include <string.h>

#include "json.h"
#include "fct_utiles.h"
#include "myCRC.h"

int scanForArrays(char* data);
int getObjectDataSize(char* data);

struct jsonObject* mkObject(char* jsonReceivedData);
struct jsonArrayElem* mkArray(char* jsonReceivedData);
void jsonObject_addData(struct jsonObject* obj, char* name, jsonObjectType type, char* data);


struct json* mkJsonData(char* jsonReceivedData)
{
	printf("Found %d arrays\n", scanForArrays(jsonReceivedData));
	printf("Main object is %d size\n", getObjectDataSize(jsonReceivedData));
	printf("Second object is %d size\n", getObjectDataSize(jsonReceivedData+20));
	printf(" : %s", jsonReceivedData+20);

	struct json* toReturn = (struct json*) malloc(sizeof(struct json));

	unsigned int i = 0;

	while(jsonReceivedData[i] != '{'){ i++; }
	toReturn->mainObject = mkObject(jsonReceivedData+i);
	
	return 0;
}

struct jsonObject* mkObject(char* jsonReceivedData)
{
	printf("jsonObject:\n%s\n", jsonReceivedData);
	
	int i = 0;
	int objectLevel = 0;	// Root level == 0
	struct jsonObject* currentObject = (struct jsonObject*) malloc(sizeof(struct jsonObject));

	while(jsonReceivedData[i] != '\0')
	{
		switch(jsonReceivedData[i]) {
		case '{':	// Object open
			if(objectLevel == 0) { objectLevel++; }	// Beginning of the object
			else { mkObject(jsonReceivedData+i); }
			break;
		case '}':
			objectLevel--;
		default:
			break;
		}
	}

	return 0;
}

struct jsonArrayElem* mkArray(char* jsonReceivedData)
{
	return 0;
}


// Object functions
// Add a "struct jsonData_elem" to a "struct jsonObject" for single data type
void jsonObject_addData(struct jsonObject* obj, char* name, jsonObjectType type, char* data)
{
	struct jsonData_elem* dataToAdd = (struct jsonData_elem*) malloc(sizeof(struct jsonData_elem));	// Allocating memory for the jsonData_elem structure
	// Setting values for the dataToAdd structure --> Single object //
	dataToAdd->arrayData = 0;	// Not an array
	dataToAdd->objectData = 0;	// Not an object
	dataToAdd->next = 0;		// No next values (at this time)

	// Setting the name
	dataToAdd->name = (char*) malloc(strlen(name)+1);
	strcpy(dataToAdd->name, name);
	// Creating the CRC for the name
	dataToAdd->namecrc = mkMyCRC(name, strlen(name)+1);
	// Setting the type of the data
	dataToAdd->dataType = type;
	// Adding the data
	dataToAdd->data = (char*) malloc(strlen(data)+1);

	// Finally adding the recently created structure to the "jsonObject*"
	struct jsonData_elem* currentElem = obj->data;
	if(data != 0)	// If there were already data
	{
		while(currentElem->next != 0)
		{ currentElem = currentElem->next; }
		currentElem->next = dataToAdd;
	}
	else { obj->data = dataToAdd; }
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
