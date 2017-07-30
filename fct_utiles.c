#include <string.h>

#include "fct_utiles.h"

// Return -1 if an error (not found)
int strcut(char* origstr, char* toSearch, char* out)
{
	char* targetLocation = strstr(origstr, toSearch);
	if(targetLocation == 0)
	{ return 0; }
	else
	{
		strcpy(out, targetLocation);
		return 0;
	}
}
