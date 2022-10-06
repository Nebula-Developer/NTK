#include <stdio.h>
#include "lib/lib.h"

int main(int argc, char **argv) {
    if (has_arg(argc, argv, "-h") || has_arg(argc, argv, "--help") || argc < 2) {
        printf("Usage: %s [options]\n", argv[0]);
        printf("Options:\n");
        printf("  -h, --help\t\tShow this help message\n");
        printf("  -c, --gcc\t\tCompile a file via em with gcc\n");
        printf("  -cp, --gpp\t\tCompile a file via em with gcc\n");
    }

    bool gcc = has_arg(argc, argv, "-c") || has_arg(argc, argv, "--gcc");
    bool gpp = has_arg(argc, argv, "-cp") || has_arg(argc, argv, "--gpp");

    if (gcc && gpp) {
        printf("You can't use both gcc and g++ at the same time!\n");
        return 1;
    }

    char *fcmp = (char *) malloc(1024);

    if (gcc)
        fcmp = get_arg_value(argc, argv, "-c") != NULL ? get_arg_value(argc, argv, "-c") : get_arg_value(argc, argv, "--gcc");
    else if (gpp)
        fcmp = get_arg_value(argc, argv, "-cp") != NULL ? get_arg_value(argc, argv, "-cp") : get_arg_value(argc, argv, "--gpp");

    if (fcmp == NULL) {
        printf("You must specify a file to compile!\n");
        return 1;
    }

    char *fileoutname = (char *) malloc(1024);

    for (int i = 0; i < strlen(fcmp) - 2; i++)
        fileoutname[i] = fcmp[i];


    char *cmd = (char *) malloc(1024);

    if (gcc)
        sprintf(cmd, "gcc -o %s %s", fileoutname, fcmp);
    else if (gpp)
        sprintf(cmd, "g++ -o %s %s", fileoutname, fcmp);

    return system(cmd);
}