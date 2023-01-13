#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int isFile(const char* path){
	struct stat path_stat;
	printf("%s path in isFile function\n", path);
	stat(path, &path_stat);
	if(S_ISREG(path_stat.st_mode))
	{
		return 1;
	}
	else if(S_ISDIR(path_stat.st_mode))
	{
		return 0;	 
	}
	else
	{
		return -1;
	}
}


void file_searcher(const char* initpath)
{
		char buff[512];
		sprintf(buff, "touch %stemppaths.txt && ls -1 %s >> %stemppaths.txt", initpath, initpath, initpath);
		system(buff);
}	

char* parse_paths(char* line, const char* path, int lineindex)
{
	FILE *pFile;
	char buff[512];
	char buff2[512];
	printf("%lu\t%lu", sizeof(line), sizeof(buff2));
	printf("%s path in parsepaths \n", path);
	sprintf(buff, "%stemppaths.txt", path);
	pFile = fopen(buff, "r");
	if (pFile==NULL) perror("error opening file");
	
	else
	{
		int i;
		for(i = 0; i < lineindex; i = i+1){

			if(fgets(buff2, 512, pFile) == NULL) break;
			else
			{
				strcpy(line, buff2);
				printf("%s line in for loop\n", line);
			}
		}
			
	}

	fclose(pFile);
	printf("%s parse_paths line \n",line);
	return line;
}

int file_line_count(const char* path)
{
	FILE *fp;
	int count = 0;
	char c;
	char buff[512];
	sprintf(buff, "%stemppaths.txt", path);
	fp = fopen(buff, "r");

	if (fp == NULL)
	{
		return 0;
	}

	for (c = fgetc(fp); c!=EOF; c = fgetc(fp))
	{
		if(c == '\n')
		{
			//printf("%d", count);
			count = count +1;
		}
	}
	fclose(fp);
	//printf("%d", count);
	return count;


}
void json_finserter(char line[])
{
	char buff[512];
	sprintf(buff, "echo \"{\\\"path\\\":\\\"%s\\\", \\\"isdir\\\":0, \\\"objnum\\\":0, \\\"subdir\\\":[]}\" >> filesystem.json", line);
	system(buff);
}

void json_dinserter(const char* path)
{
	char buff[512];
	char file_paths[512];
	sprintf(file_paths, "rm %stemppaths.txt", path);
	file_searcher(path);
	int objnum = file_line_count(path);
	char lines[objnum][512];
	printf("%lu\n", sizeof(lines[0]));
	char directories[objnum][512];
	sprintf(buff, "echo \"{\\\"path\\\":\\\"%s\\\", \\\"isdir\\\":1, \\\"objnum\\\":%d, \\\"subdir\\\":[\" >> filesystem.json", path, objnum);
	system(buff);
	printf("%d\n", objnum);
	int i;
	int n = 0;

	for(i = 0; i < objnum; i = i+1)
	{
		printf("%lu\n", sizeof(lines[i]));
		strcpy(lines[i] , parse_paths(lines[i], path, i));
		printf("%s lines[i]\n", lines[i]);
		printf("%d isFile\n", isFile(lines[i]));
		if(isFile(lines[i]) == 1)
		{
			json_finserter(lines[i]);
			system("echo , >> filesystem.json");
		}
		else if (isFile(lines[i]) == 0)
		{
			strcpy(directories[n], lines[i]);
			n++;			
		}
		else
		{
			break;
		}
	}
	
	int j;
	
	for(j = 0; j < n; j = j+1)
	{
		char new_path[512];
		sprintf(new_path, "%s/%s", path, directories[j]); 
		json_dinserter(new_path);
	}
	system("echo ]} >> filesystem.json");
	system(file_paths);

}

int main()
{
	system("echo { > filesystem.json");
	json_dinserter("filesystem");
	system("echo } >> filesystem.json");

	return 0;
}
