#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>


int main(int argc, const char** argv, const char** envp)
{
	DIR* dir;
	struct dirent* curr_file_info;
	char* full_path = NULL;
	if(argc < 2)
	{
		printf("No arguments supplied\n");
		return -1;
	}
	dir = opendir(argv[1]);
	if(dir == NULL)
	{
		printf("Error - %d\n", errno);
		return -2;
	}
	full_path = realpath(argv[1], NULL);
	while( ( curr_file_info = readdir(dir) ) != NULL )
	{
		printf("%s/%s\n", full_path, curr_file_info->d_name);	
	}
	free(full_path);	
	closedir(dir);
	printf("%s %s\n", argv[0], argv[1]);
	return 0;
}
