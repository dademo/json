#include <stdio.h>
#include <string.h>
#include <stdlib.h>	// malloc()
#include <string.h>

#include "json.h"
#include "fct_utiles.h"
#include "myCRC.h"

int scanForArrays(char* data);
int getObjectDataSize(char* data);	// Include '{' and '}'

struct jsonObject* mkObject(char* jsonReceivedData);
struct jsonArrayElem* mkArray(char* jsonReceivedData);
void jsonObject_addData(struct jsonObject* obj, char* name, enum jsonObjectType type, char* data);


struct json* mkJsonData(char* jsonReceivedData)
{
	printf("Found %d arrays\n", scanForArrays(jsonReceivedData));
	printf("Main object is %d size\n", getObjectDataSize(jsonReceivedData));
	printf("Second object is %d size\n", getObjectDataSize(jsonReceivedData+10));
	printf(" : %s", jsonReceivedData+20);

	struct json* toReturn = (struct json*) malloc(sizeof(struct json));

	unsigned int i = 0;

	while(jsonReceivedData[i] != '{'){ i++; }
	//toReturn->mainObject = mkObject(jsonReceivedData+i);
	
	return 0;
}

struct jsonObject* mkObject(char* jsonReceivedData)
{
	printf("jsonObject:\n%s\n", jsonReceivedData);
	
	int i = 0, j = 0;
	char tmpName[100] = "";	// To store name temporarily
	char tmpData[500] = "";	// To store data temporarily
	enum jsonObjectType tmpType = jsondata_unknown;
	struct jsonObject* currentObject = (struct jsonObject*) malloc(sizeof(struct jsonObject));

	while(jsonReceivedData[i] != '\0' && jsonReceivedData[i] != '{') { i++; }

	while(jsonReceivedData[i] != '\0')
	{
		switch(jsonReceivedData[i])
		{
		case ',':	// end of data, beggining of name
			while(jsonReceivedData[i] != '"' && jsonReceivedData[i] != '\0'){ i++; }
			i++;	// We are on the first '"'
			j = 0;
			while(jsonReceivedData[i] != '"' && jsonReceivedData[i] != '\0')	// While not reached the end of the name, the second '"'
			{
				tmpName[j] = jsonReceivedData[i];
				i++; j++;
			}
			break;
		case ':':	// Data
			while(jsonReceivedData[i] == ' ' && jsonReceivedData[i] != '\0'){ i++; }
			if(jsonReceivedData[i] == '{')
			{
				currentObject->data->objectData = mkObject(jsonReceivedData+i);
				i += getObjectDataSize(jsonReceivedData+i) - 1;
			}
			else
			{
				i++;	// To begin the name gestion
				j = 0;
				if(jsonReceivedData[i] == '"') { tmpType = jsondata_string; i++; }
				while(jsonReceivedData[i] != ',' && ((tmpType == jsondata_string)? jsonReceivedData[i] != '"' : 1 ) )
				{
					tmpData[j] = jsonReceivedData[i];
				}
				if(tmpType == jsondata_string){ i++; };	// We ignore the second '"' too
				// Creating the current data
				jsonObject_addData(currentObject, tmpName, tmpType, tmpData);
				// Reset data
				tmpData[0] = '\0';
				tmpName[0] = '\0';
				tmpType = jsondata_unknown;
			}
			
			break;
		}
		i++;
	}
	return 0;
}

struct jsonArrayElem* mkArray(char* jsonReceivedData)
{
	return 0;
}


// Object functions
// Add a "struct jsonData_elem" to a "struct jsonObject" for single data type
void jsonObject_addData(struct jsonObject* obj, char* name, enum jsonObjectType type, char* data)
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
			if(dataObjectLevel == 1){ return (int) (i - initPos - 1); }
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
