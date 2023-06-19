#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc , char *argv[])
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid > 0) {
        printf("parent waiting...\n");
        wait(&status);
    }
    else if(pid == 0) {
        sleep(1);
        printf("child: i'm child\n");
    }
    else {
        printf("parent: fork failed\n");
    }

    printf((pid == 0) ? "child bye\n" : "parent bye\n");

    return 0;
}

