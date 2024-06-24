#ifndef CONFIG_H
#define CONFIG_H

#include <limits.h>

#define CONFIG_FILE ".config/cview_config"
#define INITIAL_SIZE 10

struct Config {
    char email[100];  // Adjust size as needed
    char exclude[INITIAL_SIZE][50];  // Assuming only two exclude entries
    char projects[INITIAL_SIZE][PATH_MAX];  // Array of project paths
};

void initialize_config(void);
//int read_config(const char *filename);
void create_config(const char *filename);
static const char* get_home_directory();
static const char* get_config_file_path();
static int file_exists (const char *filename);

#endif // CONFIG_H

     
