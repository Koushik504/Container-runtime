#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#define STACK_SIZE (1024 * 1024)
#define MAX_CONTAINERS 10

char child_stack[STACK_SIZE];

struct container_args {
    char *rootfs;
    char *cmd;
    char *arg;
};

struct container {
    char id[32];
    pid_t pid;
};

struct container containers[MAX_CONTAINERS];
int container_count = 0;

int container_main(void *arg) {
    struct container_args *args = (struct container_args *)arg;

    printf("Inside container!\n");

    sethostname("container", 9);

    if (chroot(args->rootfs) != 0) {
        perror("chroot failed");
        return 1;
    }

    chdir("/");

    if (mount("proc", "/proc", "proc", 0, NULL) != 0) {
        perror("mount failed");
        return 1;
    }

    char *cmd[] = {args->cmd, args->arg, NULL};

    if (execvp(cmd[0], cmd) == -1) {
        perror("exec failed");
        exit(1);
    }

    return 0;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage\n");
        return 1;
    }

    // START
    if (strcmp(argv[1], "start") == 0) {

        if (argc < 6) {
            printf("Usage: start <id> <rootfs> <cmd> <arg>\n");
            return 1;
        }

        struct container_args args;
        args.rootfs = argv[3];
        args.cmd = argv[4];
        args.arg = argv[5];

        int flags = CLONE_NEWUTS | CLONE_NEWNS;

        pid_t pid = clone(container_main,
                          child_stack + STACK_SIZE,
                          flags | SIGCHLD,
                          &args);

        if (pid < 0) {
            perror("clone failed");
            return 1;
        }

        strcpy(containers[container_count].id, argv[2]);
        containers[container_count].pid = pid;
        container_count++;

        printf("Started container %s with PID %d\n", argv[2], pid);
        return 0;
    }

    // PS
   system("ps aux | grep sleep | grep -v grep");

    // STOP
    if (strcmp(argv[1], "stop") == 0) {
        for (int i = 0; i < container_count; i++) {
            if (strcmp(containers[i].id, argv[2]) == 0) {
                kill(containers[i].pid, SIGKILL);
                printf("Stopped %s\n", argv[2]);
            }
        }
        return 0;
    }

    return 0;
}
