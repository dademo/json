#include <stdio.h>
#include <stdlib.h>	// free()
#include <string.h>

#include "chainedlist.h"


struct chainedlist_char* chainedlist_char_init()
{
	struct chainedlist_char* toReturn = (struct chainedlist_char*)malloc(sizeof(struct chainedlist_char));
	toReturn->first = 0;
	toReturn->count_chainedlist_char_elem = 0;

	return toReturn;
}

void chainedlist_char_addElem(struct chainedlist_char* head, char* data)
{
	struct chainedlist_char_elem* current = head->first;
	if(current != 0) // If there's already a value
	{
		while (current->next != 0)
		{ current = current->next; }

		// Creating the next chainedlist_char_elem
		// having the memory space
		current->next = (struct chainedlist_char_elem*) malloc(sizeof(struct chainedlist_char_elem));

		current = current->next;
	}
	else	// Void list
	{
			head->first = (struct chainedlist_char_elem*) malloc(sizeof(struct chainedlist_char_elem));

		current = head->first;
	}

	// Modifying count value
	head->count_chainedlist_char_elem ++;

	// Modifying the next chainedlist_char_elem
		// charData
		current->charData = (char*) malloc(strlen(data)+1);
		strcpy(current->charData, data);
		// next
		current->next = 0;

}



void chainedlist_char_deleteStruct(struct chainedlist_char* head)
{
	struct chainedlist_char_elem* allElems[head->count_chainedlist_char_elem];
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
	free(head);
}



int chainedlist_char_deleteIndex(struct chainedlist_char* head, unsigned int index)	// Return -1 if index > head->count_chainedlist_char_elem - 1
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
}

char* chainedlist_char_getbyindex(struct chainedlist_char* head, unsigned int index)
{
	struct chainedlist_char_elem* current = head->first;
	if(head->count_chainedlist_char_elem < index || head->count_chainedlist_char_elem == 0)
	{ return 0; }
	else
	{
		for(unsigned int i = 0; i < index; i++)
		{
			current = current->next;
		}
		return current->charData;
	}
}

