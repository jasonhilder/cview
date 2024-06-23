#include "config.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// 0 == success, -1 == failed
static int file_exists(const char *filename) {
    struct stat   buffer;   
    return stat(filename, &buffer);
}

static const char* get_home_directory() {
    const char* home = getenv("HOME");

    if(home == NULL || home[0] == '\0') {
        fprintf(stderr, "ERROR: Home environment variable is not set.\n");
        exit(EXIT_FAILURE);
    }

    return home;
}

static const char* get_config_file_path() {
    static char config_path[PATH_MAX];
    const char *home = get_home_directory();
    snprintf(config_path, sizeof(config_path), "%s/%s", home, CONFIG_FILE);
    return config_path;
}


void initialize_config(void) {
    const char *config_file_path = get_config_file_path();
    // printf("CONFIG FILE!!: %s \n", config_file_path);

    // Check if this file exists otherwise create it with some defaults
    if(file_exists(config_file_path) == -1) {
        printf("File NOT exists!!\n"); 
        // prompt user for email addressed used for commits
        // prompt user for any specific folder types to ignore i.e (node_modules) 
    }
}
