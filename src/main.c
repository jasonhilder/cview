#include "config.h"
#include "git2/commit.h"
#include "git2/errors.h"
#include "git2/global.h"
#include "git2/repository.h"
#include "git2/revwalk.h"
#include "git2/types.h"
#include <git2.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void usage() {
	printf("Usage:\n");
	printf(" -a<folder_path>\n");
	printf(" -e<exclude_folder_path>\n");
	exit(EXIT_SUCCESS);
}

int is_dir(const char* fileName) {
    struct stat path;
    stat(fileName, &path);

    return S_ISREG(path.st_mode);
}


void get_git_info(const char* repo_path, const char* email) {
    int error;

    //printf("this is a git directory add to list: %s\n", repo_path);
    printf("email to check: %s\n", email);

    git_repository* repo = NULL;
    
    error = git_repository_open(&repo, repo_path);
    if(error < 0) {
        const git_error* e = git_error_last();
        fprintf(stderr, "Error opening repository: %s\n", e->message);

        git_repository_free(repo);
        git_libgit2_shutdown();
        exit(error);
    }

    // -----------------------------------------------------------
    git_revwalk *walker;
    git_commit *commit;
    git_oid oid;

    struct tm since_date = {0};
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &since_date);
    
    // create revision walker
    error = git_revwalk_new(&walker, repo);
    if(error < 0) {
        const git_error *e = git_error_last();
        fprintf(stderr, "Error creating revwalker: %s\n", e->message);

        git_repository_free(repo);
        git_libgit2_shutdown();
        exit(error);
    }

    // push head to walker and sort by time
    git_revwalk_push_head(walker);
    git_revwalk_sorting(walker, GIT_SORT_TIME);

    // iterate through commits
    while((error = git_revwalk_next(&oid, walker)) == 0) {
        error = git_commit_lookup(&commit, repo, &oid);
        if(error < 0) {
            const git_error *e = git_error_last();
            fprintf(stderr, "Error looking up commit: %s\n", e->message);
            continue;
        }
        
        // get info
        const git_signature *author = git_commit_author(commit);
        time_t commit_time = git_commit_time(commit);
        struct tm *commit_tm = gmtime(&commit_time);

        // Compare commit date and email
        if (commit_tm->tm_year >= since_date.tm_year &&
            commit_tm->tm_mon >= since_date.tm_mon &&
            commit_tm->tm_mday >= since_date.tm_mday &&
            strcmp(author->email, email) == 0) {
            
            printf("Commit: %s\n", git_commit_message(commit));
            printf("Author: %s <%s>\n", author->name, author->email);
            printf("Date: %s", asctime(commit_tm));
            printf("-----------------------------------------\n");
        }

    }

    git_revwalk_free(walker);
    // -----------------------------------------------------------
    git_repository_free(repo);
}


// Given a directory path, loop over its contents recursively
// check each directory for a .git folder, if found this is a git project.
void dir_scan(char* base_path, char** excluded_files, int excluded_len, char* email) {
    char path[PATH_MAX];
    struct dirent *dp;
    DIR *dir = opendir(base_path);

    if(dir == NULL)
        return;

    git_libgit2_init();

    while((dp = readdir(dir)) != NULL) {

        if(strcmp(dp->d_name, ".git") == 0) {
			// TODO: get its git stats and store them...
            get_git_info(base_path, email);
        }

        // @TODO: need to find a better way to ignore specified folders
        for(int i = 0; i < excluded_len; i++) {
            if ( strcmp(dp->d_name, excluded_files[i]) == 0 ) {
                continue;
            }
        }

        // Go through all subfolders
        if ( strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 )
        {
            // if this path is a directory search its contents.
            if(is_dir(path) == 0) {
                strcpy(path, base_path);
                strcat(path, dp->d_name);
                dir_scan(path, excluded_files, excluded_len, email);
            }

        }
    }

    git_libgit2_shutdown();
    
    closedir(dir);
}

// Generates the git graph of contributions
void genstats(config_t config_details) {
    printf("generate stats \n\n");

    for(int i = 0; i < config_details.project_len; i++) {
        // printf("%s\n", config_details.projects[i]);
		dir_scan(
            config_details.projects[i], 
            config_details.exclude, 
            config_details.exclude_len,
            config_details.email
        );
    }
}

int main() {
    config_t config_details = initialize_config();

    // Generate stats as graph from the config files projects array
    // genstats(config_details);

    free_config(&config_details);
}
