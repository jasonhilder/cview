/*
@TODO:
On first run check for config, if not ask user for info.
- email
- directory(s)

@NOTE:
Has a list of folders and on run goes over each path:
1. checks the directory for a .git folder
    1.1 if found gets the commits for this folder
    1.2 if not found continues to loop over the sub dirs to check for .git folders
    1.3 if no .git folders found in directory and sub directory it just returns

2. genstats loops over a predefined list of git projects
    2.1 First checks if a config file exists for the user.
        2.1.1 If not creates it
    2.2
*/

#include "config.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void usage() {
	printf("Usage:\n");
	printf(" -a<folder_path>\n");
	exit(EXIT_SUCCESS);
}

int is_dir(const char* fileName) {
    struct stat path;
    stat(fileName, &path);

    return S_ISREG(path.st_mode);
}

// @TODO: Need to loop over a list of directorys in a config file to
// gather all git projects to search
// Given a path to a directory scan its content
void dir_scan(char* base_path) {
    char path[PATH_MAX];
    struct dirent *dp;
    DIR *dir = opendir(base_path);

    // Unable to open directory stream
    if(dir == NULL)
        return;

    while((dp = readdir(dir)) != NULL) {

        // found a git project add it to the list and return to keep searching
        if(strcmp(dp->d_name, ".git") == 0) {
            printf("this is a git directory add to list: %s\n", base_path);
        }

        // Go through all subfolders
        // @TODO: need to find a better way to ignore specified folders
        if (
            strcmp(dp->d_name, ".") != 0 && 
            strcmp(dp->d_name, "..") != 0 &&
            strcmp(dp->d_name, "zig-cache") != 0 &&
            strcmp(dp->d_name, "tmp") != 0 
        )
        {
            // if this path is a directory search its contents.
            if(is_dir(path) == 0) {
                // Construct new path from our base path
                strcpy(path, base_path);
                strcat(path, dp->d_name);
                //printf("%s\n", dp->d_name);
                dir_scan(path);
            }

        }
    }
    
    closedir(dir);
}

// Search the current directory for a .git folder  
void find_git_folder(DIR *dir) {
    struct dirent *dp;
    while((dp = readdir(dir)) != NULL) {
        printf("%s\n", dp->d_name);

    }
}

// Generates the git graph of contributions
void genstats() {
    printf("generate stats \n");
}

int main(int argc, char *argv[]) {

    initialize_config();

    /*
    // printf("Program Name: %s \n", argv[0]);
    if(argc > 1 && argv[1][0] == '-') {
        switch(argv[1][1]) {
            case 'a':
                // printf("searching folder(s): %s\n", argv[2]);
                // dir_scan(argv[2]);
                break;
            default:
                printf("Incorrect argument: %s\n", argv[1]);
                usage();
        }
    }

    // Generate stats as graph from the config files projects array
    // Also checks and generates a config if not present
    genstats();
    */
}
