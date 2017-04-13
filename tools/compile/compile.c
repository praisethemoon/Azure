/*
	Script compiler for Windows: compiles scripts into stand-alone executables.
*/

#include <windows.h>
#include <stdio.h>

long flengthh(FILE* file)
{
	long cur = ftell(file), len;
	fseek(file, 0, SEEK_END);
	len = ftell(file);
	fseek(file, cur, SEEK_SET);
	return len;
}

long flength(char* fname)
{
	FILE* file = fopen(fname, "rb");
	long flen;
	flen = flengthh(file);
	fclose(file);
	return flen;
}

int main(int argc, char* argv[])
{
	HANDLE hResource;
	char lpBuffer[1024*100] = {0};
	long size = flength(argv[2]);
	FILE* file = fopen(argv[2], "rb");
	BOOL res;
	DWORD error;
	fread(lpBuffer, sizeof(char), 1024*100, file);
	fclose(file);
	hResource = BeginUpdateResource(argv[1], FALSE);
	error = GetLastError();
	if (NULL != hResource)
	{
		res = UpdateResource(hResource, 
			RT_RCDATA, 
			MAKEINTRESOURCE(9511079), 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			(LPVOID)lpBuffer, 
			size);
		error = GetLastError();
		res = EndUpdateResource(hResource, FALSE);
		error = GetLastError();
	}
	return 0;
}
