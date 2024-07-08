#include "config.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// 0 == success, -1 == failed
int file_exists(const char *filename) {
    struct stat   buffer;   
    return stat(filename, &buffer);
}

char* get_home_directory() {
    char* home = getenv("HOME");

    if(home == NULL || home[0] == '\0') {
        fprintf(stderr, "ERROR: Home environment variable is not set.\n");
        exit(EXIT_FAILURE);
    }

    return home;
}


char* get_config_file_path() {
    char *config_path = malloc(PATH_MAX);
    const char *home = get_home_directory();

    snprintf(config_path, PATH_MAX, "%s/%s", home, CONFIG_FILE);

    return config_path;
}

void create_config(const char *config_path) {
    const char* home = getenv("HOME");
    char input[PATH_MAX];

    char* default_excludes[EXCLUDE_SIZE_DEFAULT] = {
        "vendor",  
        "node_modules", 
        "zig-cache",
        "tmp"
    };

    // Write the config to a file
    FILE *fp = fopen(config_path, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file");
        exit(EXIT_FAILURE);
    }

    printf("Enter email: ");
    fgets(input, sizeof(input), stdin); 
    fprintf(fp, "email:\n%s\n---", input);

    printf("Default exclude folders (vendor, node_modules, zig-cache, tmp).\nAdd new excluded folders with -e.\n");

    fprintf(fp, "\nexclude:\n");
    for(int i = 0; i < EXCLUDE_SIZE_DEFAULT; i++) {
        if(strlen(default_excludes[i]) > 0) {
            fprintf(fp, "%s\n", default_excludes[i]);
        }
    }

    fprintf(fp, "---\nprojects:\n");
    printf("Please add projects (enter \".done\" to stop):\n");
    
    while(1) {
        // Read input from stdin (keyboard)
        fgets(input, sizeof(input), stdin); 

        // Remove newline character if present
        input[strcspn(input, "\n")] = '\0';

        if(strcmp(input, ".done") == 0) {
            break;
        }

        // Write input to file
        fprintf(fp, "%s%s\n", home, input);
        printf("Add another project or enter \".done\" to stop:\n");
    }

    fclose(fp);
    printf("Config file created successfully.\n");
}

config_t read_config(char *config_path) {
    struct Config config_details;

    // Initialize config_details
    config_details.exclude = NULL;
    config_details.exclude_len = 0;
    config_details.projects = NULL;
    config_details.project_len = 0;
    memset(config_details.email, 0, sizeof(config_details.email));

    FILE *fp = fopen(config_path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file");
        exit(EXIT_FAILURE);
    }
       
    int i = 0;
    int total = 0;

    int read_size = 256; 
    int line_size = PATH_MAX; 
    char line[read_size][line_size];  
	int lines_stored = 0;

    while(fgets(line[i], line_size, fp)) {
        line[i][strlen(line[i]) - 1] = '\0';
		lines_stored++;
        i++;
    }
    
    total = i;
    for(i = 0; i < total; i++) {
        if(strcmp(line[i], "") == 0)
            continue;

        if(strcmp(line[i], "email:") == 0) {
            i++;
            strncpy(config_details.email, line[i], strlen(line[i]));
        }


        if(strcmp(line[i], "exclude:") == 0) {
            i++;
            while(strcmp(line[i], "---") != 0) {

                if (config_details.exclude_len == 0) {
                    config_details.exclude = malloc(sizeof(char *));
                } else {
                    config_details.exclude = realloc(config_details.exclude, sizeof(char *) * (config_details.exclude_len + 1));
                }
                if(config_details.exclude == NULL) {
                    fprintf(stderr, "Failed to allocate memory for struct");
                }

                config_details.exclude[config_details.exclude_len] = strdup(line[i]);
                config_details.exclude_len++;
                i++;
            }
        }

        if(strcmp(line[i], "projects:") == 0) {
            i++;
            while(i < lines_stored) {

                if (config_details.project_len == 0) {
                    config_details.projects = malloc(sizeof(char *));
                } else {
                    config_details.projects = realloc(config_details.projects, sizeof(char *) * (config_details.project_len + 1));
                }
                // check for allocation error;
                if(config_details.projects == NULL) {
                    fprintf(stderr, "Failed to allocate memory for struct");
                }

                config_details.projects[config_details.project_len] = strdup(line[i]);
                config_details.project_len++;
                i++;
            }
        }
    }

    fclose(fp);
    return config_details;
}

void free_config(config_t *config) {
    for (int i = 0; i < config->exclude_len; i++) {
        free(config->exclude[i]);
    }
    free(config->exclude);

    for (int i = 0; i < config->project_len; i++) {
        free(config->projects[i]);
    }
    free(config->projects);
}

config_t initialize_config(void) {
    char* config_file_path = get_config_file_path();

    if(file_exists(config_file_path) == -1) {
        create_config(config_file_path);
    }
      
    config_t config_details = read_config(config_file_path);

    free(config_file_path);
    return config_details;
}
