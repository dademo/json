#include <stdio.h>
#include <unistd.h>

#include "chainedlist.h"

int main()
{
	printf ("-----BEGIN-----\n");
	struct chainedlist_char* myList = chainedlist_char_init();

	printf("-----ADDING-----\n");

	char myCharTest[] = "Coucoueuh !";
	char myCharTest2[] = "Coucou 2 !";
	char myCharTest3[] = "Coucou 3 !";

	chainedlist_char_addElem(myList, myCharTest);
	chainedlist_char_addElem(myList, myCharTest2);
	chainedlist_char_addElem(myList, myCharTest3);
printf("----------\n");
	printf("Get : %s\n", chainedlist_char_getbyindex(myList, 0));
	printf("Get : %s\n", chainedlist_char_getbyindex(myList, 1));
	printf("Get : %s\n", chainedlist_char_getbyindex(myList, 2));
printf("----------\n");
	chainedlist_char_deleteIndex(myList, 1);
printf("----------\n");
	printf("Get : %s\n", chainedlist_char_getbyindex(myList, 0));
	printf("Get : %s\n", chainedlist_char_getbyindex(myList, 1));
printf("----------\n");
	chainedlist_char_deleteIndex(myList, 0);
printf("----------\n");
	printf("Get : %s\n", chainedlist_char_getbyindex(myList, 0));
printf("----------\n");
	chainedlist_char_deleteStruct(myList);

	printf("----- END -----\n");

	return 0;
}
