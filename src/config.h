#ifndef CONFIG_H
#define CONFIG_H


#define CONFIG_FILE ".config/cview_config"

void initialize_config(void);
//int read_config(const char *filename);
//int write_config(const char *filename);
static const char* get_home_directory();
static const char* get_config_file_path();
static int file_exists (const char *filename);

#endif // CONFIG_H

     
