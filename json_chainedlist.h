#ifndef JSON_CHAINEDLIST_H_INCLUDED
#define JSON_CHAINEDLIST_H_INCLUDED

#include "json.h"

struct jsonData* chainedlist_char_init();
struct jsonData_elem* jsondatalist_addElem(struct jsonData* list, char* data);
void jsondatalist_deleteStruct(struct jsonData* list);
//int jsondatalist_deleteIndex(struct jsonData* list, unsigned int index);	// Return -1 if index > head->count_chainedlist_char_elem - 1
struct jsonData_elem* jsondatalist_getbyindex(struct jsonData* list, unsigned int index);

#endif
