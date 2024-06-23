/*
@TODO:
On first run check for config, if not ask user for info.
- email
- directory(s)
*/

#include <stdlib.h>
#include <stdio.h>

void usage() {
	printf("Usage:\n");
	printf(" -a<folder_path>\n");
	exit(EXIT_SUCCESS);
}


// Given a path to a directory scan its content
void scandir(char* path) {
    printf("scan\n");
}


// Generates the git graph of contributions
void genstats() {
    printf("generate stats \n");
}


int main(int argc, char *argv[]) {

    printf("Program Name: %s \n", argv[0]);

    if(argc > 1 && argv[1][0] == '-') {
        switch(argv[1][1]) {
            case 'a':
                printf("folder to add: \n");
                printf("%s\n", argv[2]);
                break;
            default:
                printf("Incorrect argument: %s\n", argv[1]);
                usage();
        }
    }

}
