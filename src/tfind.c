#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include "lib/lib.h"
#include "lib/dir.h"

int main(int argc, char **argv) {
    if (has_arg(argc, argv, "-h") || has_arg(argc, argv, "--help") || argc < 2) {
        printf("Usage: %s [options] <filename>\n", argv[0]);
        printf("Options:\n");
        printf("  -h, --help\t\tShow this help message\n");
        printf("  -a, --all\t\tSearch all directories, including hidden.\n");
        printf("  -n, --nosubdirs\tDon't search subdirectories\n");
        printf("  -sd, --searchdir\tSearch a specific directory\n");
        return 0;
    }

    bool nosubdirs = has_arg(argc, argv, "-n") || has_arg(argc, argv, "--nosubdirs");
    bool all = has_arg(argc, argv, "-a") || has_arg(argc, argv, "--all");
    char *filename = argv[1];

    char *dir = (char *) malloc(1024);
    if (has_arg(argc, argv, "-sd") || has_arg(argc, argv, "--searchdir")) {
        dir = get_arg_value(argc, argv, "-sd") != NULL ? get_arg_value(argc, argv, "-sd") : get_arg_value(argc, argv, "--searchdir");
    }
    else {
        getcwd(dir, 1024);
    }

    if (dir[strlen(dir) - 1] != '/') {
        strcat(dir, "/");
    }

    char **files = readd(dir, !all, NULL, !nosubdirs);
    int count = 0;

    for (int i = 0; i < 2048; i++) {
        if (files[i] == NULL) {
            continue;
        }
        char* slash = strrchr(files[i], '/');
        char* file = slash ? slash + 1 : files[i];

        if (strcmp(file, filename) == 0) {
            printf("%s%s\n", dir, files[i]);
            count++;
        }
    }
}