/******************************************************************************
* Copyright (C) 2005 Gangcai Lin
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
******************************************************************************/

#ifndef _AZURE_H_
#define _AZURE_H_

/*
 * To build a suitable profile of the language to meet specific requirements,
 * define/undefine the corresponding compiling directives as needed:
 *
 * AZ_ENABLE_DYNAMIC_LOADING: enable dynamic module loading functionality;
 * AZ_ENABLE_WARNING: print warning messages;
 * AZ_ENABLE_TRAP: enable instruction interrupt handler functionality;
 * AZ_ENABLE_OPTIMIZATION: enable optimizations;
 * AZ_MAC_OSX: define this when compiled under Mac OSX;
 * AZ_CPLUSPLUS: define this when compiled along with a C++ project;
*/

#define AZ_ENABLE_DYNAMIC_LOADING
#define AZ_ENABLE_OPTIMIZATION
/* #define AZ_MAC_OSX */

#ifdef AZ_CPLUSPLUS
extern "C"
{
#endif

#include <stdio.h>
#ifndef AZ_MAC_OSX
#include <malloc.h>
#endif

#define AZ_START_RESOURCE_INDEX 9511079

#define AZ_DEFAULT_STACK_SIZE 1024		/* stack size in #words */
#define AZ_DEFAULT_HEAP_SIZE 1024*500	/* heap size in #bytes */

/* Forward definitions. */
struct _Azure_entity;
struct _avm;
struct _ctable;
struct _word;

typedef unsigned char uchar;
typedef unsigned int uint;

/* Prototype for native functions and callback functions. */
typedef void (*userfunc)(struct _avm* vm);

/* Compile and execute APIs. */
struct _Azure_entity* az_compile(const char* source);
void az_callf(struct _Azure_entity* entity, const char* func);
int az_runstring(const char* source, const char* str);
int az_runfile(const char* filename);

/* APIs for manipulating the Azure virtual machine. */
/* Push operations. */
void az_pushi(struct _Azure_entity* entity, int ival);
void az_pushf(struct _Azure_entity* entity, float fval);
void az_pushs(struct _Azure_entity* entity, const char* str);
/* Get return values. */
int az_getreti(struct _Azure_entity* entity, int n);
float az_getretf(struct _Azure_entity* entity, int n);
char* az_getrets(struct _Azure_entity* entity, int n);
/* Get global variables. */
int az_getglobali(struct _Azure_entity* entity, const char* idname);
float az_getglobalf(struct _Azure_entity* entity, const char* idname);
char* az_getglobals(struct _Azure_entity* entity, const char* idname);
/* Set global variables. */
void az_setglobali(struct _Azure_entity* entity, const char* idname, int ival);
void az_setglobalf(struct _Azure_entity* entity, const char* idname, float fval);
void az_setglobals(struct _Azure_entity* entity, const char* idname, const char* sval);
/* Others. */
void az_settraphook(struct _Azure_entity* entity, userfunc hook);
void az_release_entity(struct _Azure_entity* entity);

/* Library extension functions. */
void az_regfunc(userfunc uf, char* fname);
userfunc az_getfunc(char* name);
void az_openlibs();
void az_closelibs();

/* Library functions. */
void az_openlibstd();
void az_openlibio();
void az_openlibmath();
void az_openlibstr();
void az_openlibsys();
#ifdef AZ_ENABLE_DYNAMIC_LOADING
void az_opendllib();
#endif

/* Console functions. */
void az_prompt(int argc, char *argv[]);

#ifdef AZ_CPLUSPLUS
}
#endif

#endif
