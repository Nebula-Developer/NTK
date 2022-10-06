#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "lib/lib.h"
#include <libproc.h>
#include <errno.h>
#include <signal.h>
#include <sys/proc_info.h>
#include <sys/proc.h>
#include <string.h>

int main(int argc, char **argv) {
    if (has_arg(argc, argv, "-h") || has_arg(argc, argv, "--help")) {
        printf("Usage: %s [options]\n", argv[0]);
        printf("Options:\n");
        printf("  -h, --help\t\tShow this help message\n");
        printf("  -p, --path\t\tPrint the path of each process\n");
        printf("  -np, --needpath\t\tSkip if path is not found\n");
        printf("  -v, --verbose\t\tShow verbose output\n");
    }

    bool verbose = has_arg(argc, argv, "-v") || has_arg(argc, argv, "--verbose");

    // List every process
    int numprocs = proc_listallpids(NULL, 0);

    pid_t pids[numprocs];
    bzero(pids, sizeof(pids));
    proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));

    if (verbose) {
        printf("Found %d processes:\n", numprocs);
    }

    for (int i = 0; i < numprocs; i++) {
        if (verbose) {
            printf("    Process %d: \n", pids[i]);
        }

        char name[100];
        bzero(name, sizeof(name));
        proc_name(pids[i], name, sizeof(name));

        if (name[0] != '\0') {
            if (has_arg(argc, argv, "-p") || has_arg(argc, argv, "--path")) {
                char path[PROC_PIDPATHINFO_MAXSIZE];
                bzero(path, sizeof(path));
                proc_pidpath(pids[i], path, PROC_PIDPATHINFO_MAXSIZE);

                if (path[0] == '\0' && (has_arg(argc, argv, "-np") || has_arg(argc, argv, "--needpath"))) {
                    if (verbose) {
                        printf("skipped (no path) %d\n", pids[i]);
                    }
                    continue;
                }

                printf("%d:\t%s -> %s\n", pids[i], name, (path[0] != '\0') ? path : "unknown");
            } else {
                printf("%d:\t%s\n", pids[i], name);
            }
        }
    }

    return 0;
}