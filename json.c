#include <stdio.h>
#include <string.h>
#include <stdlib.h>	// malloc(), atoi()
#include <string.h>

#include "json.h"
#include "fct_utiles.h"
#include "myCRC.h"

int scanForArrays(char* data);
int getObjectDataSize(char* data);	// Include '{' and '}'
int getArrayDataSize(char* data);	// Include '[' and ']'

struct jsonObject* mkObject(char* jsonReceivedData);
struct jsonArrayElem* mkArray(char* jsonReceivedData);
void jsonObject_addData(struct jsonObject* obj, char* name, enum jsonObjectType type, char* data);
void jsonObject_addObjectData(struct jsonObject* obj, char* name, struct jsonObject* object);
void jsonObject_addArrayData(struct jsonObject* obj, char* name, struct jsonArrayElem* array);

struct json* mkJsonData(char* jsonReceivedData)
{
	printf("Found %d arrays\n", scanForArrays(jsonReceivedData));
	printf("Main object is %d size\n", getObjectDataSize(jsonReceivedData));
	printf("Second object is %d size\n", getObjectDataSize(jsonReceivedData+10));
	printf("Main array is %d size\n", getArrayDataSize(jsonReceivedData));

	struct json* toReturn = (struct json*) malloc(sizeof(struct json));

	unsigned int i = 0;

	while(jsonReceivedData[i] != '{'){ i++; }
	toReturn->mainObject = mkObject(jsonReceivedData+i);
	
	return toReturn;
}

struct jsonObject* mkObject(char* jsonReceivedData)
{
	//printf("jsonObject:\n%s\n", jsonReceivedData);
	printf("\n-----NEW OBJECT-----\n");
	fflush(stdout);


	int i = 0, j = 0;
	int alreadyInObject = 0;
	char tmpName[100] = "";	// To store name temporarily
	char tmpData[500] = "";	// To store data temporarily
	enum jsonObjectType tmpType = jsondata_unknown;
	struct jsonObject* currentObject = (struct jsonObject*) malloc(sizeof(struct jsonObject));
	currentObject->data = 0;	// No data in the beginning

	while(jsonReceivedData[i] != '\0' && jsonReceivedData[i] != '{') { i++; }

	while(jsonReceivedData[i] != '\0' && jsonReceivedData[i] != '}')
	{
		switch(jsonReceivedData[i])
		{
		case '{':
			if(alreadyInObject == 0){ alreadyInObject++; }
			else { break; }
		case ',':	// end of data, beginning of name
			while(jsonReceivedData[i] != '"' && jsonReceivedData[i] != '\0'){ i++; }
			i++;	// We are on the first '"'
			j = 0;
			while(jsonReceivedData[i] != '"' && jsonReceivedData[i] != '\0')	// While not reached the end of the name, the second '"'
			{
				tmpName[j] = jsonReceivedData[i];
				i++; j++;
			}
			tmpName[j] = '\0';
			printf("tmpName : %s\t-\t", tmpName);
			break;
		case ':':	// end of name, beginning of data
			while(jsonReceivedData[i] == ' ' && jsonReceivedData[i] != '\0'){ i++; }
			i++;
			if(jsonReceivedData[i] == '{')	// If object's data is an object too
			{
				jsonObject_addObjectData(currentObject, tmpName, mkObject(jsonReceivedData+i));
				//currentObject->data->objectData = mkObject(jsonReceivedData+i);
				i += getObjectDataSize(jsonReceivedData+i) - 1;
				currentObject->data->arrayData = 0;	// Not an array
			}
			else if(jsonReceivedData[i] == '[')	// If object's data is an array
			{
				jsonObject_addArrayData(currentObject, tmpName, mkArray(jsonReceivedData+i));
				//currentObject->data->arrayData = mkArray(jsonReceivedData+i);
				i += getArrayDataSize(jsonReceivedData+i) - 1;
				currentObject->data->objectData = 0;	// Not an object
			}
			else
			{
				j = 0;
				if(jsonReceivedData[i] == '"') { tmpType = jsondata_string; i++; }
				while((tmpType == jsondata_string)? jsonReceivedData[i] != '"' : jsonReceivedData[i] != ',') 
				{
					tmpData[j] = jsonReceivedData[i];
					i++; j++;
				}
				i--;	// To not to go too far
				tmpData[j] = '\0';
				printf("tmpData : %s\n", tmpData);
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

	printf("\n-----END OF OBJECT-----\n");

	return currentObject;
}

struct jsonArrayElem* mkArray(char* jsonReceivedData)
{
	printf("\n-----NEW ARRAY-----\n");
	//printf("jsonArrayElem:\n%s\n", jsonReceivedData);
	fflush(stdout);

	struct jsonArrayElem* currentArray = (struct jsonArrayElem*) malloc(sizeof(struct jsonArrayElem)), *tempElem = currentArray;
	currentArray->elem = 0;
	currentArray->next = 0;

	int i = 0;
	while(jsonReceivedData[i] != '[') { i++; }	// beginning of the array

	i++;	

	while(jsonReceivedData[i] != ']')
	{
		switch(jsonReceivedData[i])
		{
		case '{':
			if(tempElem->elem == 0) { tempElem->elem = mkObject(jsonReceivedData+i); }	// First of the array, no data set here
			else
			{
				tempElem->next = (struct jsonArrayElem*) malloc(sizeof(struct jsonArrayElem));
				tempElem = tempElem->next;
				tempElem->elem = mkObject(jsonReceivedData + i);
			}
			break;
		}
		i++;
	}

	printf("\n-----END OF ARRAY-----\n");

	return currentArray;
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
	strcpy(dataToAdd->data, data);

	// Finally adding the recently created structure to the "jsonObject*"
	struct jsonData_elem* currentElem = obj->data;
	if(obj->data != 0)	// If there were already data
	{
		while(currentElem->next != 0)
		{ currentElem = currentElem->next; }
		currentElem->next = dataToAdd;
	}
	else { obj->data = dataToAdd; }
} 

void jsonObject_addObjectData(struct jsonObject* obj, char* name, struct jsonObject* object)
{
	struct jsonData_elem* dataToAdd = (struct jsonData_elem*) malloc(sizeof(struct jsonData_elem));	// Allocating memory for the jsonData_elem structure
	// Setting values for the dataToAdd structure --> Single object //
	dataToAdd->data = 0;		// No single data inside
	dataToAdd->arrayData = 0;	// Not an array
	dataToAdd->next = 0;		// No next values (at this time)

	// Setting the name
	dataToAdd->name = (char*) malloc(strlen(name)+1);
	strcpy(dataToAdd->name, name);
	// Creating the CRC for the name
	dataToAdd->namecrc = mkMyCRC(name, strlen(name)+1);
	// Setting the type of the data
	dataToAdd->dataType = jsondata_object;
	// Adding the data
	dataToAdd->objectData = object;

	// Finally adding the recently created structure to the "jsonObject*"
	struct jsonData_elem* currentElem = obj->data;
	if(obj->data != 0)	// If there were already data
	{
		while(currentElem->next != 0)
		{ currentElem = currentElem->next; }
		currentElem->next = dataToAdd;
	}
	else { obj->data = dataToAdd; }

}


void jsonObject_addArrayData(struct jsonObject* obj, char* name, struct jsonArrayElem* array)
{
	struct jsonData_elem* dataToAdd = (struct jsonData_elem*) malloc(sizeof(struct jsonData_elem));	// Allocating memory for the jsonData_elem structure
	// Setting values for the dataToAdd structure --> Single object //
	dataToAdd->data = 0;		// No single data inside
	dataToAdd->objectData = 0;	// Not an object
	dataToAdd->next = 0;		// No next values (at this time)

	// Setting the name
	dataToAdd->name = (char*) malloc(strlen(name)+1);
	strcpy(dataToAdd->name, name);
	// Creating the CRC for the name
	dataToAdd->namecrc = mkMyCRC(name, strlen(name)+1);
	// Setting the type of the data
	dataToAdd->dataType = jsondata_array;
	// Adding the data
	dataToAdd->arrayData = array;

	// Finally adding the recently created structure to the "jsonObject*"
	struct jsonData_elem* currentElem = obj->data;
	if(obj->data != 0)	// If there were already data
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

int getArrayDataSize(char* data)	// Include '[' and ']'
{
	int dataArrayLevel = 0;
	
	int initPos = 0;
	for(initPos = 0; data[initPos] != '['; initPos++){}

	for(size_t i = 0; i < strlen(data); i++)
	{
		if(data[i] == '['){ dataArrayLevel++; }
		if(data[i] == ']')
		{
			if(dataArrayLevel == 1){ return (int) (i - initPos - 1); }
			dataArrayLevel--;
		}
	}
	return -1;
}


struct jsonData_elem* getObjectElem(struct jsonObject* object, const char* elemName);
struct jsonData_elem* getInArrayElem(struct jsonArrayElem* firstElem, int index, const char* elemName);

struct jsonData getJsonData(struct json* jsondata, const char* path)
{
	#define STRUCTJSONDATA_ERROR (struct jsonData) {0,0}


	printf("Getting : %s\n", path);

	//struct jsonObject* current = jsondata->mainObject;
	struct jsonData_elem* current = 0;

	int i = 0, j = 0;
	int arglvl = 0;
	char buff[50] = "";	// For the given argument
	int isArray = 0;

	while(path[i] != '\0')
	{
		// Getting argument
		while(path[i] != '.' && path[i] != '\0')
		{
			if(path[i] == '[')	// Requesting an array
			{
				char nbrbuff[10];
				char objbuff[50];
				int k = 0;
				int index;
				i++;
				while(path[i] != ']')
				{
					nbrbuff[k] = path[i];
					i++; k++;
				}
				nbrbuff[k] = '\0';
				index = atoi(nbrbuff);
				// Getting the element to search in the array
				i+=2;	// On '.'
				k = 0;
				while(path[i] != '.' && path[i] != '\0')
				{
					if(path[i] == '\\') { i++; }	// If there's an \, we ignore the next letter
					objbuff[k] = path[i];
					i++; k++;
				}
				objbuff[k] = '\0';
				printf("In array (%s[%d]), seeking %s\n", buff, index, objbuff);

				if(current == 0 && arglvl == 0)
				{
					if(jsondata->mainObject != 0)
					{ current = getObjectElem(jsondata->mainObject, buff); }
					else { printf("Error, no object at %s\n", current->name); return STRUCTJSONDATA_ERROR; }
				}
				else if(current != 0) {
					if(current->objectData != 0)
					{ current = getObjectElem(current->objectData, buff); }
					else { printf("Error, no object at %s\n", current->name); return STRUCTJSONDATA_ERROR; }
				}
				else { printf("Error, current == 0... (loop %d)\n", arglvl); return STRUCTJSONDATA_ERROR; }	// Error

				if(current != 0) {
					if(current->arrayData != 0) {
						current = getInArrayElem(current->arrayData, index, objbuff);
					}
					//else { printf("Error, no array at %s\n", current->name); return 0; }
					else { printf("Error, no array at %s\n", current->name); return STRUCTJSONDATA_ERROR; }
				}
				//else { printf("Error, current == 0... (loop %d)\n", arglvl); return 0; }	// Error
				else { printf("Error, current == 0... (loop %d)\n", arglvl); return STRUCTJSONDATA_ERROR; }	// Error





				isArray = 1;	// To say the treatment have already been done because it's an array
			}
			else
			{
				if(path[i] == '\\') { i++; }	// If there's an \, we ignore the next letter
				buff[j] = path[i];
				i++; j++;
			}
		}
		buff[j] = '\0';
		if(current == 0)
		{
			if(arglvl == 0)	// If it's the first search
			{
				current = getObjectElem(jsondata->mainObject, buff);
			}
			else { printf("Error, current == 0... (loop %d)\n", arglvl); return STRUCTJSONDATA_ERROR; }	// Error
		}
		else
		{
			if(isArray == 0)
			{
				printf("Arg %d : %s\n", arglvl, buff);

				if(current->objectData != 0)
				{ current = getObjectElem(current->objectData, buff); }
				else { printf("Error, no object at %s\n", current->name); return STRUCTJSONDATA_ERROR; }
			}
		}
		
		arglvl++;
		buff[0] = '\0';
		isArray = 0;
		j = 0;
		i++;
	}

	if(current == 0)
	{ return STRUCTJSONDATA_ERROR; }
	else if (current->data == 0)
	{ return STRUCTJSONDATA_ERROR; }
	else
	{ return (struct jsonData) {current->name, current->data}; }
}

struct jsonData_elem* getObjectElem(struct jsonObject* object, const char* elemName)
{
	struct jsonData_elem* current = object->data;
	int64_t namecrc = mkMyCRC((void*) elemName, strlen(elemName)+1);

	while(current != 0)
	{
		if(current->namecrc == namecrc)
		{
			if(strcmp(current->name, elemName) == 0)
			{ return current; }
		}
		current = current->next;
	}

	return 0;
}

struct jsonData_elem* getInArrayElem(struct jsonArrayElem* firstElem, int index, const char* elemName)
{
	struct jsonArrayElem* current = firstElem;
	struct jsonData_elem* currentElem = 0;
	int64_t namecrc = mkMyCRC((void*) elemName, strlen(elemName)+1);

	for(int i = 0; i < index && current != 0; i++)
	{
		current = current->next;
	}
	if(current == 0) { return 0; }
	else {
		currentElem = current->elem->data;
		while(currentElem != 0)
		{
			if(currentElem->namecrc == namecrc)
			{
				if(strcmp(currentElem->name, elemName) == 0)
				{ return currentElem; }
			}
			currentElem = currentElem->next;
		}
	}

	return 0;
}

void readObject(struct jsonObject* object);
void readArray(struct jsonArrayElem* array);

void readAllJsonData(struct json* data)
{
	printf("\n\n-----READING DATA-----\n\n");

	readObject(data->mainObject);

	printf("\n\n----------END----------\n\n");
}

void readObject(struct jsonObject* object)
{
	enum todo { todo_none, todo_object, todo_array };

	enum todo next_todo = todo_none;

	printf("-----OBJECT-----\n");

	struct jsonData_elem* elem = object->data;

	while(elem != 0)
	{
		printf("\
Element :\
\n\t\
Name : %s\
\n\tNamecrc : %li\n", elem->name, elem->namecrc);
		if (elem->data != 0) { printf("\tData : %s\n", elem->data); }
		else { printf("\tData : 0\n"); }
		if (elem->arrayData != 0) { printf("\tArrayData : %p\n", (void*) elem->arrayData); next_todo = todo_array; }
		else { printf("\tArrayData : 0\n"); }
		if (elem->objectData != 0) { printf("\tObjectData : %p\n", (void*) elem->objectData); next_todo = todo_object; }
		else { printf("\tObjectData : 0\n"); }
		if (elem->next != 0) { printf("\tNext : %p\n", (void*) elem->next); }
		else { printf("\tNext : 0\n"); }

		switch(next_todo)
		{
		case todo_object:
			readObject(elem->objectData);
			break;
		case todo_array:
			readArray(elem->arrayData);
			break;
		default:
			printf("Plus rien à faire ...\n");
		}

		elem = elem->next;
	}

	printf("-----OBJECT END-----\n");
}


void readArray(struct jsonArrayElem* array)
{
	printf("-----ARRAY-----\n");

	struct jsonArrayElem* current = array;
	if(array != 0) {
	while(current->next != 0)
	{
		readObject(current->elem);
		current = current->next;

	}
	}

	printf("-----ARRAY END-----\n");
}


void delJsonObject(struct jsonObject* object);
void delJsonArray(struct jsonArrayElem* firstElem);
int getJsonObjectCount(struct jsonObject* object);
int getJsonArrayCount(struct jsonArrayElem* firstElem);

void freeJsonData(struct json* dataroot)
{
	delJsonObject(dataroot->mainObject);
	free(dataroot);
}


void delJsonObject(struct jsonObject* object)
{
	int elementsCount = getJsonObjectCount(object);
	struct jsonData_elem* allElements[elementsCount];
	struct jsonData_elem* current = object->data;

	for(int i = 0; i < elementsCount; i++)	// Getting all the values of the object
	{
		allElements[i] = current;
		current = current->next;
	}

	for(int i = 0; i < elementsCount; i++)	// Freeing all the elements in the structure
	{
		if(allElements[i] != 0)
		{
			if(allElements[i]->name != 0) { free(allElements[i]->name); }
			if(allElements[i]->data != 0) { free(allElements[i]->data); }
			if(allElements[i]->arrayData != 0) { delJsonArray(allElements[i]->arrayData); free(allElements[i]->arrayData); }
			if(allElements[i]->objectData != 0) { delJsonObject(allElements[i]->objectData); free(allElements[i]->objectData); }
			free(allElements[i]);
		}
	}

	free(object);	// Freeing the memory of the object itself, because there's nothing more to free
}

void delJsonArray(struct jsonArrayElem* firstElem)
{
	int elementsCount = getJsonArrayCount(firstElem);
	struct jsonArrayElem* allElements[elementsCount];
	struct jsonArrayElem* current = firstElem;

	for(int i = 0; i < elementsCount; i++)	// Getting all the elements of the array
	{
		allElements[i] = current;
		current = current->next;
	}

	for(int i = 0; i < elementsCount; i++)	// Freeing the object in the array element
	{
		if(allElements[i] != 0)
		{
			if(allElements[i]->elem != 0) { delJsonObject(allElements[i]->elem); }
			free(allElements[i]);
		}
		
	}
}


int getJsonObjectCount(struct jsonObject* object)
{
	int count = 0;
	struct jsonData_elem* current = object->data;

	while(current != 0)
	{
		current = current->next;
		count++;
	}
	return count;
}

int getJsonArrayCount(struct jsonArrayElem* firstElem)
{
	int count = 0;
	struct jsonArrayElem* current = firstElem;

	while(current != 0)
	{
		current = current->next;
		count++;
	}
	return count;
}
