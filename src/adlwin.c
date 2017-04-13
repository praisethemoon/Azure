/*
 * Dynamic loading of modules for Windows.
 * See Copyright Notice in azure.h.
*/

#include <windows.h>
#include "azure.h"
#include "avm.h"
#include "aexception.h"
#include "aerror.h"

#ifdef AZ_ENABLE_DYNAMIC_LOADING

/* Open a dynamic linking library. */
void wdlopen(avm* vm)
{
	word w;
	char* lib = getstring(vm, 0);
	HMODULE mod = LoadLibrary((LPCTSTR)lib);
	if (!mod)
		setvmerror(vm, AZ_VM_CANNOT_LOAD_LIB);
	seti(&w, (int)mod);
	returnv(vm, &w);
}

/* Find a function in a module by the function's name. */
void wdlfind(avm* vm)
{
	word w;
	HMODULE mod = (HMODULE)getint(vm, 0);
	char* func = getstring(vm, 1);
	userfunc pfunc = (userfunc)GetProcAddress(mod, func);
	if (!pfunc)
	{
		FreeLibrary(mod);
		setvmerror(vm, AZ_VM_CANNOT_FIND_FUNC);
	}
	settypew(&w, THFUNCTION);
	w.entity.ival = (int)pfunc;
	returnv(vm, &w);
}

/* Derefer a dynamic linking library. */
void wdlclose(avm* vm)
{
	HMODULE mod = (HMODULE)getint(vm, 0);
	if (mod) FreeLibrary(mod);
}

void az_opendllib()
{
	az_regfunc(wdlopen, "dlopen");
	az_regfunc(wdlfind, "dlfind");
	az_regfunc(wdlclose, "dlclose");
}

#endif
