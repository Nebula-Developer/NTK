#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "lib/lib.h"
#include <libproc.h>
#include <errno.h>
#include <signal.h>

int proc_find(char *procname);

int main(int argc, char **argv) {
    if (has_arg(argc, argv, "-h") || has_arg(argc, argv, "--help") || argc < 2) {
        printf("Usage: %s <procname> [options]\n", argv[0]);
        printf("Options:\n");
        printf("  -h, --help\t\tShow this help message\n");
        printf("  -s, --signal\t\tSend a signal to the process\n");
        printf("  -p, --pid\t\tGet the PID of the process\n");
        printf("  -k, --kill\t\tKill the process\n");
        printf("  -r, --restart\t\tRestart the process\n");
        return 0;
    }

    bool gsignal = has_arg(argc, argv, "-s") || has_arg(argc, argv, "--signal");
    bool gpid = has_arg(argc, argv, "-p") || has_arg(argc, argv, "--pid");
    bool gkill = has_arg(argc, argv, "-k") || has_arg(argc, argv, "--kill");
    bool grestart = has_arg(argc, argv, "-r") || has_arg(argc, argv, "--restart");

    char *procname = argv[1];

    // try to find a process with the given name
    int pid = proc_find(procname);
    if (pid == -1) {
        printf("Process not found\n");
        return 1;
    }
    else {
        if (gpid) {
            printf("%d\n", pid);
        }

        if (gsignal) {
            char *sig = get_arg_value(argc, argv, "-s") != NULL ? get_arg_value(argc, argv, "-s") : get_arg_value(argc, argv, "--signal");
            int signal = atoi(sig);

            if (signal == 0) {
                printf("Invalid signal\n");
                return 1;
            }
            else {
                int ret = kill(pid, signal);
                if (ret == -1) {
                    printf("Failed to send signal: %s\n", strerror(errno));
                    return 1;
                }
            }
        }

        if (gkill) {
            int ret = kill(pid, SIGKILL);
            if (ret == -1) {
                printf("Failed to kill process: %s\n", strerror(errno));
                return 1;
            }
        }

        if (grestart) {
            int ret = kill(pid, SIGKILL);
            if (ret == -1) {
                printf("Failed to kill process: %s\n", strerror(errno));
                return 1;
            }

            char *cmd = (char *)ntk_malloc(sizeof(char) * 100);
            sprintf(cmd, "open -a %s", procname);
            system(cmd);
        }

        return 0;
    }
    return 0;
}

int proc_find(char *procname) {
    int pid = -1;
    int ret = proc_listpids(PROC_ALL_PIDS, 0, NULL, 0);
    if (ret <= 0) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }
    int *pids = malloc(ret * sizeof(int));
    ret = proc_listpids(PROC_ALL_PIDS, 0, pids, ret * sizeof(int));
    if (ret <= 0) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }
    for (int i = 0; i < ret; i++) {
        char *name = malloc(1024);
        proc_name(pids[i], name, 1024);
        if (strcmp(name, procname) == 0) {
            pid = pids[i];
            break;
        }
    }
    return pid;
}