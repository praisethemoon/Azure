/*
 * The Azure console for Linux.
 * See Copyright Notice in azure.h.
*/

#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include "alib.h"
#include "azure.h"
#include "autil.h"
#include "aconsole.h"

void az_prompt(int argc, char *argv[])
{
	char* lpBuffer;
	char buffer[10240] = {0};
	size_t size;
	char* pos;
	az_openlibs();
	if (argc <= 1)
	{
		printf(AZ_CON_HEADER);
		while (1)
		{
			printf(">>");
			fgets(buffer, 1024, stdin);
			strcut(buffer, " \t\n\r");
			if ((pos=strstr(buffer, ".run")) == buffer)
			{
				pos += strlen(".run")+1;
				az_runfile(pos);
			}
			else if (strcmp(buffer, ".exit") == 0)
				break;
			else if (strcmp(buffer, ".help") == 0)
				printf(AZ_CON_HELP);
			else if (strlen(buffer) > 0)
				az_runstring("", buffer);
			printf("\n");
		}
	}
	else	/* command-line mode */
		az_runfile(argv[1]);
	az_closelibs();
}
