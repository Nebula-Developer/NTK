#include <stdio.h>
#include "lib/lib.h"
#include <sys/stat.h>

int main(int argc, char **argv) {
    if (has_arg(argc, argv, "-h") || has_arg(argc, argv, "--help") || argc < 2) {
        printf("Usage: %s <file> [options]\n", argv[0]);
        printf("Options:\n");
        printf("  -h, --help\t\tShow this help message\n");
        printf("  -s, --simple\t\tSimplify the output\n");
        return 0;
    }

    bool simple = has_arg(argc, argv, "-s") || has_arg(argc, argv, "--simple");

    char *file = argv[1];

    // Get permissions with <sys/stat.h>
    struct stat st;
    stat(file, &st);
    mode_t perm = st.st_mode;

    char *owner = (char *) malloc(1024);
    char *group = (char *) malloc(1024);
    char *other = (char *) malloc(1024);

    sprintf(owner, "%s%s%s", (perm & S_IRUSR) ? "r" : "-", (perm & S_IWUSR) ? "w" : "-", (perm & S_IXUSR) ? "x" : "-");
    sprintf(group, "%s%s%s", (perm & S_IRGRP) ? "r" : "-", (perm & S_IWGRP) ? "w" : "-", (perm & S_IXGRP) ? "x" : "-");
    sprintf(other, "%s%s%s", (perm & S_IROTH) ? "r" : "-", (perm & S_IWOTH) ? "w" : "-", (perm & S_IXOTH) ? "x" : "-");

    // Print permissions)
    if (simple) {
        printf("OWN=%s\nGRP=%s\nOTH=%s\n", owner, group, other);
    } else {
        printf("Permissions for %s:\n", file);
        printf("  Owner: %s\n", owner);
        printf("  Group: %s\n", group);
        printf("  Other: %s\n", other);
    }

    return 0;
}