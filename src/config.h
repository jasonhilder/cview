#ifndef CONFIG_H
#define CONFIG_H

#include <limits.h>

#define CONFIG_FILE ".config/cview_config"

#define INITIAL_SIZE 10
#define EMAIL_LEN 256
#define EXCLUDE_SIZE_DEFAULT 4

typedef struct Config {
    char** exclude; 
    int exclude_len;

    char** projects;
    int project_len;

    char email[EMAIL_LEN]; 
} config_t;

config_t initialize_config(void);
config_t read_config(const char *filename);
void create_config(const char *filename);
void free_config(config_t *config);
const char* get_home_directory();
const char* get_config_file_path();
int file_exists (const char *filename);

#endif // CONFIG_H
     
