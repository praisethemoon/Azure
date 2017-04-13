/*
 * System library.
 * See Copyright Notice in azure.h.
*/

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "azure.h"
#include "alib.h"
#include "atable.h"

/*
	The time function returns the number of seconds elapsed since midnight
	(00:00:00), January 1, 1970, coordinated universal time, according to
	the system clock.
*/
void cctime(avm* vm)
{
	time_t ltime;
	word w;
	time(&ltime);
	seti(&w, (int)ltime);
	returnv(vm, &w);
}

void csystem(avm* vm)
{
	word w={0};
	char* str = getstring(vm, 0);
	if (strcmp(str, ""))
		seti(&w, system((const char*)str));
	returnv(vm, &w);
}

void cpackf(avm* vm)
{
	word w={0};
	long toff = (long)vm->tot.table[getarg(vm, 0)->entity.ival].offset;
	ctable* table = (ctable*)getdata(vm->hp.heap, toff);
	seti(&w, (int)packf(vm, table));
	returnv(vm, &w);
}

static fptrname libsys[] =
{
	{cctime, "time"},
	{csystem, "system"},
	{cpackf, "packf"},
};

void az_openlibsys()
{
	int i;
	for (i=0; i<sizeof(libsys)/sizeof(fptrname); i++)
		az_regfunc(libsys[i].ptr, libsys[i].name);
}
