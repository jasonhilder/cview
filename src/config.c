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

void create_config(const char *config_path) {
    struct Config config = {
        .email = "",
        .exclude = {
            "vendor",  
            "node_modules", 
            "zig-cache",
            "tmp"
        },
        .projects = {}
    };

    // Write the config to a file
    FILE *fp = fopen(config_path, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file");
        exit(EXIT_FAILURE);
    }

    // Write email
    printf("Enter email: ");
    fgets(config.email, sizeof(config.email), stdin);
    fprintf(fp, "email:\n%s\n", config.email);

    // Write excludes
    int elen = sizeof(config.exclude) / sizeof(config.exclude[0]);
    fprintf(fp, "exclude:\n");
    printf("Add excluded folder (type .n): ");
    for(int i = 0; i < elen; i++) {
        if(strlen(config.exclude[i]) > 0) {
            fprintf(fp, "%s\n", config.exclude[i]);
        }

    }

    // Write projects
    fprintf(fp, "\nprojects:\n");

    fclose(fp);
    printf("Config file created successfully.\n");
}


void initialize_config(void) {
    const char *config_file_path = get_config_file_path();
    // printf("CONFIG FILE!!: %s \n", config_file_path);

    // Check if this file exists otherwise create it with some defaults
    if(file_exists(config_file_path) == -1) {
        // printf("File NOT exists!!\n"); 
        create_config(config_file_path);
    }
      
    // read config and return it as a struct
}
