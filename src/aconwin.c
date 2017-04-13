/*
 * The Azure console for Windows.
 * See Copyright Notice in azure.h.
*/

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include "azure.h"
#include "autil.h"
#include "aconsole.h"

void az_prompt(int argc, char *argv[])
{
	HRSRC hResource;
	HGLOBAL hResourceLoaded;
	char* lpBuffer;
	char buffer[10240] = {0};
	size_t size;
	char* pos;
	HMODULE hLibrary;
	az_openlibs();
	if (argc <= 1)
	{
		hLibrary = GetModuleHandle(NULL);
		hResource = FindResource(hLibrary,
			MAKEINTRESOURCE(AZ_START_RESOURCE_INDEX), RT_RCDATA);
		if (NULL != hResource)	/* stand-alone executable */
		{
			hResourceLoaded = LoadResource(hLibrary, hResource);
			if (NULL != hResourceLoaded)
			{
				lpBuffer = (char*)LockResource(hResourceLoaded);
				if (NULL != lpBuffer)
				{
					size = SizeofResource(hLibrary, hResource);
					memcpy(buffer, lpBuffer, size);
					az_runstring("", buffer);
				}
			}
		}
		else	/* interactive mode */
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
		FreeLibrary(hLibrary);
	}
	else	/* command-line mode */
		az_runfile(argv[1]);
	az_closelibs();
}
