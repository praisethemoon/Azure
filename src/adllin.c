/*
 * Dynamic loading of modules for Linux.
 * See Copyright Notice in azure.h.
*/

#include <dlfcn.h>
#include "azure.h"
#include "avm.h"
#include "alib.h"
#include "aexception.h"
#include "aerror.h"

#ifdef AZ_ENABLE_DYNAMIC_LOADING

/* Open a dynamic library. */
void ldlopen(avm* vm)
{
	word w;
	char* lib = getstring(vm, 0);
	void* handle = dlopen(lib, RTLD_LAZY);
	if (!handle)
		setvmerror(vm, AZ_VM_CANNOT_LOAD_LIB);
	seti(&w, (int)handle);
	returnv(vm, &w);
}

/* Find a function in a module by the function's name. */
void ldlfind(avm* vm)
{
	word w;
	void* handle = (void*)getint(vm, 0);
	char* func = getstring(vm, 1);;
	userfunc pfunc = dlsym(handle, func);
	if (!pfunc)
	{
		dlclose(handle);
		setvmerror(vm, AZ_VM_CANNOT_FIND_FUNC);
	}
	settypew(&w, THFUNCTION);
	w.entity.ival = (int)pfunc;
	returnv(vm, &w);
}

/* Derefer a dynamic library. */
void ldlclose(avm* vm)
{
	void* handle = (void*)getint(vm, 0);
	if (handle) dlclose(handle);
}

void az_opendllib()
{
	az_regfunc(ldlopen, "dlopen");
	az_regfunc(ldlfind, "dlfind");
	az_regfunc(ldlclose, "dlclose");
}

#endif
