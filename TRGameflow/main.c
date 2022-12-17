#include "ReadScript.h"
#include "WriteScript.h"
#include "helpers.h"

int main(int argc, char* argv[], char* env[])
{
	FILE* fp;
	char name[50];

	//Get input file
	memset(name, 0, 50);
	printf("Enter script file name: ");
	scanf("%s", name);

	fp = fopen(name, "r");

	if (!fp)
	{
		printf("Could not open file.");
		return;
	}

	//read the script
	ReadScript(fp);
	fclose(fp);

	//Get output file
	RenameExtension(name, ".dat");
	fp = fopen(name, "wb");

	if (!fp)
	{
		printf("Could not create file.");
		return;
	}

	//write the script
	WriteScriptFile(fp);
	fclose(fp);
}
