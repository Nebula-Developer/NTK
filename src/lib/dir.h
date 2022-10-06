#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

#ifndef NTKLIB_H
#include "lib.h"
#endif

#define malloc(x) ntk_malloc(x)

char **readd(char *p, bool ignore_hidden, char **exts) {
    DIR *d;
    struct dirent *dir;
    d = opendir(p);

    char **files = ntk_cha_malloc(sizeof(char *) * 2048);
    int i = 0;

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) {
                if (exts != NULL) {
                    char *ext = strrchr(dir->d_name, '.');

                    if (ext != NULL) {
                        ext++;
                        if (in_array(ext, exts)) {
                            files[i] = dir->d_name;
                            i++;
                        }
                    }
                } else {
                    files[i] = dir->d_name;
                    i++;
                }
            } else if (dir->d_type == DT_DIR) {
                if (ignore_hidden && dir->d_name[0] == '.') {
                    continue;
                }

                if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                    // Set cwd to the directory
                    char *cwd = ntk_chp_malloc(sizeof(char) * 2048);
                    getcwd(cwd, 2048);
                    chdir(dir->d_name);

                    char **subfiles = readd(".", ignore_hidden, exts);
                    int j = 0;
                    while (subfiles[j] != NULL) {
                        char *name = ntk_chp_malloc(sizeof(char) * 2048);
                        strcpy(name, dir->d_name);
                        strcat(name, "/");
                        strcat(name, subfiles[j]);
                        files[i] = name;

                        i++;
                        j++;
                    }

                    // Set cwd back to the original
                    chdir(cwd);
                }
            }
        }
        closedir(d);
    }
    return files;
}