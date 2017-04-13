/* See Copyright Notice in azure.h. */

#include "azure.h"

int main(int argc, char *argv[])
{
/*
	struct _Azure_entity* entity;
	int g1, g2;
	float g3;
	az_openlibs();
	entity = az_compile("../tests/embed.az");
	az_setglobali(entity, "g1", 5);
	az_setglobali(entity, "g2", 123);
	az_pushf(entity, 4.5);
	az_pushi(entity, 1234);
	az_callf(entity, "sub");
	g1 = az_getglobali(entity, "g1");
	g2 = az_getglobali(entity, "g2");
	g3 = az_getretf(entity, 0);
	az_pushs(entity, "Denis");
	az_callf(entity, "hi");
	az_closelibs();
	az_release_entity(entity);
*/
	az_prompt(argc, argv);
	return 0;
}
