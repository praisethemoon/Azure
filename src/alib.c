/*
 * Library manipulation functions.
 * See Copyright Notice in azure.h.
*/

#include <stdlib.h>
#include <string.h>
#include "azure.h"
#include "alib.h"

userlib ulib = {0};

/* Register a user-defined function to the library. */
void az_regfunc(userfunc uf, char* fname)
{
	int newsize;
	fptrname* newlib;
	if (ulib.nextslot>=ulib.size)	/* the array full? */
	{
		newsize = ulib.size*2+1;
		if (!(newlib=calloc(newsize,sizeof(fptrname))))
			return;
		memcpy(newlib, ulib.lib, ulib.size*sizeof(fptrname));
		if (ulib.lib)
			free(ulib.lib);
		ulib.lib = newlib;
		ulib.size = newsize;
	}
	ulib.lib[ulib.nextslot].ptr = uf;
	ulib.lib[ulib.nextslot++].name = fname;
}

/* Get a function's entry by its name. */
userfunc az_getfunc(char* name)
{
	int i;
	for (i=0; i<ulib.nextslot; i++)
		if (strcmp(ulib.lib[i].name, name) == 0)
			return ulib.lib[i].ptr;
	return 0;
}

void az_openlibs()
{
	az_openlibstd();
	az_openlibio();
	az_openlibmath();
	az_openlibstr();
	az_openlibsys();
#ifdef AZ_ENABLE_DYNAMIC_LOADING
	az_opendllib();
#endif
}

void az_closelibs()
{
	free(ulib.lib);
}
