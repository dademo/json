#ifndef JSON_H_DEFINED
#define JSON_H_DEFINED

#include <stdint.h>	// int64_t

// Type of the JSON data, for the information
enum jsonObjectType {
	jsondata_string,	// char*
	jsondata_null,		// == 0
	jsondata_bool,		// boolean (int : 0<>1)
	jsondata_array,		// array of jsonData
	jsondata_nbr,		// double (char*)
	jsondata_object,	// struct jsonObject
	jsondata_unknown	// default, if doesn't know
};

// JSON data structure
struct json {
	struct jsonObject* mainObject;	// Main object of the JSON structure
};

// JSON object structure
struct jsonObject {
	struct jsonData_elem* data;	// Chained list of struct jsonData
};

// JSON array structure (dynamic array of jsonObject*)
struct jsonArrayElem {
	struct jsonObject* elem;	// data
	struct jsonArrayElem* next;	// if == 0, no element after
};

// JSON data itself
struct jsonData_elem {
	char* name;			// name of the variable
	int64_t namecrc;		// custom checksum, for fast data get
	enum jsonObjectType dataType;	// type of data, no use but for the information
	char* data;			// data if not an array
	struct jsonArrayElem* arrayData;// if an array, pointer to the first element
	struct jsonObject* objectData;	// if data is as object, pointer to the object
	struct jsonData_elem* next;	// if == 0, no element after
};

struct jsonData {
	char* name;
	char* data;
};

/*
 *	struct json
 *	     |--> struct jsonObject
 *			|--> struct jsonData_elem (char* name)
 *				   |--> char* data
 *			    (OR)   |--> struct jsonArrayElem
 *						|--> struct jsonObject -> (...)
 *					      (...)
 *				   |............|
 *				   |
 *			    (OR)   |--> struct jsonObject
 *						|
 *					      (...)
 *				   |............|
 *			|..........|
 *	    |...........|
 *				
 */


struct json* mkJsonData(char* jsonReceivedData);
void freeJsonData(struct json* dataroot);
struct jsonData getJsonData(struct json* jsondata, const char* path);	// Dotted path (ex: 0.Data[1].x)

void readAllJsonData(struct json* data);

#endif
