#define NTKLIB_H

#ifndef bool
#define bool int
#define true 1
#define false 0
#endif

#ifndef NULL
#define null (long)0
#endif

#ifndef __DARWIN_NULL
#define __DARWIN_NULL (long)0
#endif

#include <string.h>
#include <stdlib.h>

bool has_arg(int argc, char **argv, char *arg) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], arg) == 0) {
            return true;
        }
    }
    return false;
}

char *get_arg_value(int argc, char **argv, char *arg) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], arg) == 0) {
            if (i + 1 < argc) {
                return argv[i + 1];
            } else {
                return NULL;
            }
        }
    }

    return NULL;
}

bool in_array(char *str, char **array) {
    for (int i = 0; array[i] != NULL; i++) {
        if (strcmp(str, array[i]) == 0) {
            return true;
        }
    }
    return false;
}

char **split(char *str, char *delim) {
    char **result = (char **)malloc(sizeof(char *) * 100);
    char *token = strtok(str, delim);
    int i = 0;
    while (token != NULL) {
        result[i] = token;
        token = strtok(NULL, delim);
        i++;
    }
    result[i] = NULL;
    return result;
}

void *ntk_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        printf("ntk_malloc: failed to allocate memory\n");
        exit(1);
    }
    return ptr;
}

char **ntk_cha_malloc(size_t size) {
    char **ptr = (char **)malloc(size);
    if (ptr == NULL) {
        printf("ntk_cha_malloc: failed to allocate memory\n");
        exit(1);
    }
    return ptr;
}

char *ntk_chp_malloc(size_t size) {
    char *ptr = (char *)malloc(size);
    if (ptr == NULL) {
        printf("ntk_chp_malloc: failed to allocate memory\n");
        exit(1);
    }
    return ptr;
}