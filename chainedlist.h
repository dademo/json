#ifndef CHAINEDLIST_H_INCLUDED
#define CHAINEDLIST_H_INCLUDED

struct chainedlist_char_elem {
	char* charData;			// String to store
	struct chainedlist_char_elem* next;	// Next of list
};

struct chainedlist_char {
	unsigned int count_chainedlist_char_elem;
	struct chainedlist_char_elem* first;	// First of chained list
};

struct chainedlist_char* chainedlist_char_init();
void chainedlist_char_addElem(struct chainedlist_char* list, char* data);
void chainedlist_char_deleteStruct(struct chainedlist_char* head);
int chainedlist_char_deleteIndex(struct chainedlist_char* head, unsigned int index);	// Return -1 if index > head->count_chainedlist_char_elem - 1
char* chainedlist_char_getbyindex(struct chainedlist_char* head, unsigned int index);

#endif
