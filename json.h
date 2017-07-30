#ifndef JSON_H_DEFINED
#define JSON_H_DEFINED

#include <stdint.h>	// int64_t

enum jsonObjectType {
	jsondata_string,
	jsondata_null,
	jsondata_bool,
	jsondata_array,		// array of jsonData
	jsondata_nbr,		// double
	jsondata_unknown	// default, if doesn't know
};

struct jsonData {
	struct jsonData_elem* first;
};

struct jsonData_elem {
	char* name;			// for root data, name = root
	int64_t namecrc;		// custom checksum, for fast data get
	enum jsonObjectType datatype;	// Type of data, no use but for the information
	char* data;			// data if not an array
	int countArray;			// if array, size of the array
	struct jsonData* arrayData;	// If an array, sore data here
	struct jsonData_elem* next;		// if == 0, no element after
};

struct jsonObject {
	struct jsonData* data;	// Chained list of struct jsonData
};

struct jsonObject* mkJsonData(char* jsonReceivedData);
void delJsonDataStruct(struct jsonObject* dataroot);
struct jsonObject* getJsonData(char* path);	// Dotted path (ex: 0.Data.x)

#endif
