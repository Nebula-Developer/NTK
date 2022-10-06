#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include "lib/lib.h" 
#include "lib/dir.h" 

int main(int argc, char **argv) {
    if (has_arg(argc, argv, "-h") || has_arg(argc, argv, "--help")) {
        printf("Usage: %s [options] <path>\n", argv[0]);
        printf("Options:\n");
        printf("  -h, --help\t\tShow this help message\n");
        printf("  -a, --all\t\tCount all files, including hidden files\n");
        printf("  -e, --ext\t\tCount files with the specified extension(s) (SPLIT BY ',')\n");
        printf("  -v, --verbose\t\tShow verbose output\n");
        printf("  -p, --prefix\t\tRemove the prefix from the output\n");
        return 0;
    }

    bool verbose = has_arg(argc, argv, "-v") || has_arg(argc, argv, "--verbose");

    char **exts = NULL;
    if (has_arg(argc, argv, "-e") || has_arg(argc, argv, "--ext")) {
        char *val = get_arg_value(argc, argv, "-e") != NULL ? get_arg_value(argc, argv, "-e") : get_arg_value(argc, argv, "--ext");
        // Split by comma
        exts = split(val, ",");

        if (verbose) {
            printf("Extensions: ");
            int i = 0;
            while (exts[i] != NULL) {
                printf("%s ", exts[i]);
                i++;
            }
            printf("\n");
        }
    }

    // This will count the words of all files and subdirectories
    char **files = readd(".", !(has_arg(argc, argv, "-a") || has_arg(argc, argv, "--all")), exts);
    int wordCount = 0;

    // Count the number of lines in each file
    for (int i = 0; i < 2048; i++) {
        FILE *file = fopen(files[i], "r");
        if (file == NULL) {
            continue;
        }
        if (verbose) printf("%s\n", files[i]);
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        while ((read = getline(&line, &len, file)) != -1) {
            char **words = split(line, " ");
            int j = 0;
            while (words[j] != NULL) {
                wordCount++;
                j++;
            }
        }
        if (verbose) printf("%s completed\n", files[i]);
        fclose(file);
    }

    printf("%s%d\n", (!(has_arg(argc, argv, "-p") || has_arg(argc, argv, "--prefix")) ? "Total number of words: " : ""), wordCount);
    return 0;
}