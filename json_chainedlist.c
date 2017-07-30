#include <stdio.h>
#include <stdlib.h>	// free()
#include <string.h>

#include "json_chainedlist.h"
#include "myCRC.h"


void jsondatalist_allocMemoryElement(struct jsonData_elem* elem, char* name, jsonObjectType dataType, char* data);

void jsondatalist_delArray(struct jsonData_elem* toDel);


struct jsonData* chainedlist_char_init()
{
	struct jsonData* toReturn = (struct jsonData*) malloc(sizeof(jsonData));
	toReturn->first = 0;
	return toReturn;
}

void jsondatalist_allocMemoryElement(struct jsonData_elem* elem, char* name, jsonObjectType dataType, char* data)
{

	// Allocating space to the jsonData_elem elements
	elem->name = (char*) malloc(strlen(name)+1);
		strcpy(elem->name, name);
	elem->datatype = dataType;
	elem->data = (char*) malloc(strlen(data)+1);
		strcpy(elem->data, data);
	elem->countArray = 0;
	elem->arrayData = 0;
	elem->next = 0;
}

struct jsonData_elem* jsondatalist_addElem(struct jsonData* list, char* name, jsonObjectType dataType, char* data)
{
	struct jsonData_elem* current = list->first;
	if(current != 0)
	{
		while(current->next != 0)
		{ current = current->next; }
		// Creating the next jsonData_elem
		// having the memory space
		current->next = (struct jsonData_elem*) malloc(sizeof(jsonData_elem));
		current = current->next;
	}
	else	// Void list
	{
		list->first = (struct jsonData_elem*) malloc(sizeof(jsonData_elem));
		current = list->first;
	}

	jsondatalist_allocMemoryElement(current, name, dataType, data);

	return current;
}

struct jsonData_elem* jsondatalist_elem_addElem(struct jsonData_elem* list, char* name, jsonObjectType dataType, char* data)
{
	struct jsonData_elem* current = list;
	if(current == 0)
	{
		list = (struct jsonData_elem*) malloc(sizeof(jsonData_elem));
		current = list;
	}
	else
	{
		while(current->next != 0)
		{ current = current->next; }
		current->next = (struct jsonData_elem*) malloc(sizeof(jsonData_elem));
		current = current->next;
	}
	jsondatalist_allocMemoryElement(current, name, dataType, data);
	current->namecrc = mkMyCRC(current->name, strlen(name)+1);

	return current;
}



void jsondatalist_delArray(struct jsonData_elem* toDel)
{
	// INIT //
	jsonData_elem* current = toDel;
	int count = 0;

	// Getting the element count
	if(current != 0)
	{
		jsonData_elem* current = toDel;
		count++;
		while(current->next != 0)
		{
			current = current->next;
			count++;
		}
	}
//
//
	// Populating an array with pointers to each elements of the array
	struct jsonData_elem* allElems[count];
	current = toDel;

	for(int i = 0; i < count; i++)
	{
		allElems[i] = current;
		current = current->next;
	}
//
//
	// Deleting each element of the array previously created
	for(int i = count-1; i >= 0; i++)
	{
		if(allElems[i]->arrayData != 0)
		{
			//jsondatalist_delArray(allElems[i]->arrayData->first);
			jsondatalist_deleteStruct(allElems[i]->arrayData);
			//free(allElems[i]->arrayData);
		}
		free(allElems[i]->name);
		free(allElems[i]->data);
		free(allElems[i]);
	}

}

void jsondatalist_deleteStruct(struct jsonData* list)
{

	if(list->first != 0)
	{
		jsondatalist_delArray(list->first);
		free(list->first);
	}

	/*int count = 0;
	if(list->first != 0)
	{
		jsonData_elem* current = list->first;
		count++;
		while(current->next != 0)
		{
			current = current->next;
			count++;
		}
	}

	struct jsonData_elem* allElems[count];
	
	struct jsonData_elem* current = head->first;

	if(current != 0)
	{
		
	}*/

	
	/*struct chainedlist_char_elem* allElems[head->count_chainedlist_char_elem];
	struct chainedlist_char_elem *current = head->first;
	int i = 0;
	if(current != 0)
	{
		while(current->next != 0)
		{
			allElems[i] = current;
			current = current->next;
			i++;
		}

		for(int j = i-1; j >= 0; j--)
		{
			free(allElems[j]->charData);
			if(allElems[j]->next != 0) { free(allElems[j]->next); }
		}
		free(head->first);
	}
	head->count_chainedlist_char_elem = 0;
	free(head);*/
}



/*int jsondatalist_deleteIndex(struct jsonData* head, unsigned int index)	// Return -1 if index > head->count_chainedlist_char_elem - 1
{
	struct chainedlist_char_elem* current = head->first, *old = 0;
	if(index > head->count_chainedlist_char_elem - 1)
	{
		perror("chainedlist_char_deleteIndex : Index");
		return -1;
	}
	else if(index == 0)
	{
		struct chainedlist_char_elem* temp = head->first->next;
		free(head->first->charData);
		free(head->first);
		head->first = temp;

		return 0;
	}
	else
	{
		for(unsigned int i = 0; i < index; i++)
		{
			old = current;
			current = current->next;
		}
		old->next = current->next;
		free(current->charData);
		free(current);
		
		head->count_chainedlist_char_elem --;

		return 0;
	}
}*/

jsonData_elem* jsondatalist_getbyindex(struct jsonData* head, unsigned int index)
{
	struct jsonData_elem* current = head->first;
	if(current != 0)
	{
		if(index == 0)
		{ return head->first; }
		else
		{
			unsigned int count=1;
			while(current->next != 0)
			{
				current = current->next;
				count++;
			}
			if(index > count)
			{
				return 0;
			}
			else
			{
				current = head->first;
				for(unsigned int i = 0; i < index; i++)
				{
					current = current->next;
				}
				return current;
			}
		}
	}
	else
	{ return 0; }
	
	/*struct chainedlist_char_elem* current = head->first;
	if(head->count_chainedlist_char_elem < index || head->count_chainedlist_char_elem == 0)
	{ return 0; }
	else
	{
		for(unsigned int i = 0; i < index; i++)
		{
			current = current->next;
		}
		return current->charData;
	}*/
}

