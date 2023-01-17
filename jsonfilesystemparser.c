#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

/* inserts files in filesystem into the json file in json format*/

void json_finserter(const char line[], int category)
{
	char buff[512];
	if(category == 0) sprintf(buff, "echo \"{\\\"path\\\":\\\"%s\\\", \\\"isdir\\\":0, \\\"objnum\\\":0, \\\"subdir\\\":[]}\" >> filesystem.json", line);
	else if(category == 1) sprintf(buff, "echo \"{\\\"path\\\":\\\"%s\\\", \\\"isdir\\\":0, \\\"objnum\\\":0, \\\"subdir\\\":[]},\" >> filesystem.json", line);
	system(buff);
}
/* counts the amount of objects in directory, realistically not needed, but does make somethings easier so nice to have*/

int object_count(const char* path)
{
	int count = 0;
	DIR* dp;
	struct dirent* name;

	dp = opendir(path);
	while((name = readdir(dp)) != NULL)
	{
		if(name->d_type == DT_REG || name->d_type == DT_DIR)
		{
			count++;
		}
		else
		{
			break;
		}
	}
	closedir(dp);
	return count;
}
/* inserts directories into the json file, this is the main part of the script*/

void json_dinserter(const char* path, int jin, int iin)
{
	int objnum = object_count(path) - 2;
	char buff[512];
	char directories[objnum][512];
	directories[0][0] = '\0';
	DIR* dp;
	struct dirent* name;
	int i = 0;
	int n = 0;
	sprintf(buff, "echo \"{\\\"path\\\":\\\"%s\\\", \\\"isdir\\\":1, \\\"objnum\\\":%d, \\\"subdir\\\":[\" >> filesystem.json", path, objnum);
	system(buff);

	dp = opendir(path);
	if(dp != NULL)
	{
		while((name = readdir(dp)) != NULL)
		{
			if(name->d_type == DT_REG || name->d_type == DT_DIR)
			{
				if(strcmp(name->d_name, ".") != 0 && strcmp(name->d_name, "..") != 0)
				{
					if(name->d_type == DT_REG)
					{
						if(i == objnum-1 && directories[0][0] == '\0')
						{
							json_finserter(name->d_name, 0);
						}
						else json_finserter(name->d_name, 1);
					}
					else if(name->d_type == DT_DIR)
					{
						strcpy(directories[n], name->d_name);
						n++;
					}
					i++;
				}
			}
		}
	}

	int j;
	for(j = 0; j < n; j = j+1)
	{
		char new_path[512];
		sprintf(new_path, "%s/%s", path, directories[j]);
		json_dinserter(new_path, j, n);
	}
	if( jin == iin - 1 )
	{
		system("echo ]} >> filesystem.json");
	}
	else
	{
		system("echo ]}, >> filesystem.json");
	}
}

//main
int main()
{
	system("echo {\\\"filesystem\\\": > filesystem.json");
	json_dinserter("filesystem", 0, 1);
	system("echo } >> filesystem.json");
	return 0;
}
