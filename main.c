/*
 *	Writer: Azorfus
 *	Redistributable Code
 *	Under Development!
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_BUFFER 1260

int main(int argc, char** argv)
{
	FILE* GivenFile;
	char* FileARG = argv[1];
	int LineNumber = 1;
	int Count = 0;
	bool Editing = true;
	char UserInput[128];
	char* Tok;
	int Lines[100][2];
	char* FileBuffer;
	size_t ENT = 0;


	GivenFile = fopen(FileARG, "rb+");


	// if the given file does not exist. it creates a new file
	if (GivenFile == NULL)
	{
		GivenFile = fopen("Untitled.txt", "w+");
	}

	// reading the contents of the file onto FileBuffer
	fseek(GivenFile, 0, SEEK_END);
	size_t fileSize = ftell(GivenFile);
	FileBuffer = malloc(fileSize);
	rewind(GivenFile);
	FileBuffer = realloc(FileBuffer, fileSize + ENT);
	fread(FileBuffer, 1, fileSize, GivenFile);


	char inputSec[560];
	bool inputRun = true;
	int I = 0;

	// Checking if the FileBuffer is NULL to avoid memory problems
	if (FileBuffer != NULL)
	{
		// storing newline character's index with the line number for later inserts
		for (int i = 0; i < fileSize; i++)
		{
			if (FileBuffer[i] == '\n' || FileBuffer[i] == '\0')
			{
				printf("Found at index[%d] line number[%d] char is [%c]\n", i+1, Count, FileBuffer[i]);
				Lines[Count][0] = Count+1;
				Lines[Count][1] = i+1;
				Count++;
				I = Count;
			}
			else if (FileBuffer[i+1] == '\n' || FileBuffer[i+1] == '\0')
			{
				printf("Found at index[%d] line number[%d] char is [%c]\n", i+1, Count, FileBuffer[i]);
				Lines[Count][0] = Count+2;
				Lines[Count][1] = i+2;
				Count++;
				I = Count;
			}
		}
		Lines[100][1] = 010;


		Count = 0;
		for (int i = 0; i < 100; i++)
		{
			if (i == I)
			{
				break;
			}

			printf("%d, %d\n", Lines[i][0], Lines[i][1]);
		}
	}



	while (Editing)
	{
		fputs("$ ", stdout);
		fgets(UserInput, 128, stdin);
		Tok = strtok(UserInput, ":");
		if (strcmp(Tok, "l") == 0)
		{
			fwrite(FileBuffer, sizeof(char), fileSize, stdout);
		}
		else if (strcmp(Tok, "q") == 0)
		{
			free(FileBuffer);
			fclose(GivenFile);
			exit(0);
		}
		else if (strcmp(Tok, "a") == 0)
		{
			while (inputRun)
			{
				fputs(": ", stdout);
				char* ret = fgets(inputSec, 559, stdin);
				// check ret and do something if null
				inputSec[559] = '\0'; // In case fgets reads 559 bytes without a nul
				if (strncmp(inputSec, ":END:", 5) == 0)
				{
					inputRun = false;
				}
				else {
					ENT = strlen(inputSec);
					FileBuffer = realloc(FileBuffer, fileSize + ENT);
					// copying the user input into the FileBuffer
					memcpy(FileBuffer + fileSize, inputSec, ENT);
					fileSize += ENT;
				}
			}
			inputRun = true;
		}

		else if (strcmp(Tok, "w") == 0)
		{
			fwrite(FileBuffer, sizeof(char), fileSize, GivenFile);
		}
		else if (strcmp(Tok, "i") == 0)
		{
			int LNO = 0;
			bool found;
			scanf("%d", &LNO);
			for (int i = 0; i < 100; i++)
			{
				if (LNO == Lines[i][0])
				{
					printf("FOUND at index value [%d]\n", Lines[i][1]);
				}
				else
				{
					printf("Not Found!\n");
					found = false;
				}
				if (found == true)
				{

				}
			}

		}
		else if(strcmp(Tok, "r")==0)
		{
			int LINEchsn;
			fputs(": ", stdout);
			scanf("%d", &LINEchsn);
			
			int INDEXchsn;

			if(LINEchsn >= 1 || LINEchsn < I)
			{
				INDEXchsn = Lines[LINEchsn][1];
			}
			else
			{
				printf("Line does not exist!\n");
			}

			char *lineBeginning = FileBuffer + INDEXchsn + 1;
			printf("%.*s", Lines[LINEchsn+1][1]-INDEXchsn, lineBeginning);
			int lineLength = strlen(lineBeginning);
			
			printf("fileSize: %ld\nlineBeginning - FileBuffer: %ld\nlineLength: %d\n", fileSize, (lineBeginning - FileBuffer), lineLength);

			memmove(lineBeginning, lineBeginning + lineLength, fileSize - (lineBeginning + lineLength - FileBuffer));
		}
	}

	fclose(GivenFile);

	return 0;
}
