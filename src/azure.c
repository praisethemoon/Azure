/*
 * Wrapping of compiling and running functions.
 * See Copyright Notice in azure.h.
*/

#include "azure.h"
#include "aparse.h"
#include "agen.h"
#include "avm.h"
#include "aerror.h"
#include "autil.h"

/* Compile a script ready in the buffer. */
Azure_entity* compilebuf(const char* source, char* buf)
{
	Azure_entity* entity = (Azure_entity*)calloc(1, sizeof(Azure_entity));
	parseroot(source, buf, &entity->ctx);
	if (validatecontext(&entity->ctx))
		translate(&entity->ctx, &entity->assm);
	load(&entity->ctx, &entity->vm, &entity->assm);
	return entity;
}

/* Compile a script by its name. */
Azure_entity* az_compile(const char* source)
{
	Azure_entity* entity;
	char* buf = readfile(source);
	if (!buf) return (Azure_entity*)0;
	entity = compilebuf(source, buf);
	free(buf);
	return entity;
}

void az_callf(Azure_entity* entity, const char* func)
{
	callf(&entity->vm, func);
}

/* Execute a scrippet. */
int az_runstring(const char* source, const char* str)
{
	Azure_entity* entity = compilebuf(source, (char*)str);
	run(&entity->ctx, &entity->vm);
	az_release_entity(entity);
	return 0;
}

/* Execute a script by its name. */
int az_runfile(const char* filename)
{
	char* buf = readfile(filename);
	if (!buf) return error("File not found!\n");
	az_runstring(filename, buf);
	free(buf);
	return 0;
}

void az_pushi(Azure_entity* entity, int ival)
{
	pushi(&entity->vm, ival);
}

void az_pushf(Azure_entity* entity, float fval)
{
	pushf(&entity->vm, fval);
}

void az_pushs(struct _Azure_entity* entity, const char* str)
{
	pushs(&entity->vm, str);
}

/* Get the nth return value as integer type. */
int az_getreti(struct _Azure_entity* entity, int n)
{
	word* w = getret(&entity->vm, n);
	verifytype(&entity->vm, w, TINTEGER);
	return w->entity.ival;
}

/* Get the nth return value as float type. */
float az_getretf(struct _Azure_entity* entity, int n)
{
	word* w = getret(&entity->vm, n);
	verifytype(&entity->vm, w, TFLOAT);
	return w->entity.fval;
}

/* Get the nth return value as string type. */
char* az_getrets(struct _Azure_entity* entity, int n)
{
	word* w = getret(&entity->vm, n);
	char* str = (char*)getdata(&entity->vm, w->entity.ival);
	verifytype(&entity->vm, w, TSTRING);
	return str;
}

int az_getglobali(Azure_entity* entity, const char* idname)
{
	return getglobali(&entity->vm, idname);
}

float az_getglobalf(Azure_entity* entity, const char* idname)
{
	return getglobalf(&entity->vm, idname);
}

char* az_getglobals(Azure_entity* entity, const char* idname)
{
	return getglobals(&entity->vm, idname);
}

void az_setglobali(Azure_entity* entity, const char* idname, int ival)
{
	setglobali(&entity->vm, idname, ival);
}

void az_setglobalf(struct _Azure_entity* entity, const char* idname, float fval)
{
	setglobalf(&entity->vm, idname, fval);
}

void az_setglobals(struct _Azure_entity* entity, const char* idname, const char* sval)
{
	setglobals(&entity->vm, idname, sval);
}

void az_settraphook(struct _Azure_entity* entity, userfunc hook)
{
	settraphook(&entity->vm, hook);
}

/* Release the Azure entity. */
void az_release_entity(Azure_entity* entity)
{
	deinitcontext(&entity->ctx);
	releasevm(&entity->vm);
	free(entity);
}
