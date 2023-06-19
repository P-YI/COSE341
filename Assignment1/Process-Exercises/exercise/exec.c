#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;
    printf("start\n");
    pid = fork();

    if (pid > 0) {
        printf("parent\n");
        sleep(1);
    }
    else if (pid == 0) {
        printf("child\n");
        execl("/bin/ls", "ls", NULL);
        printf("fail to execute ls -l \n");
    }
    else {
        printf("parent fail to fork\n");
    }

    printf("bye\n");
    return 0;

}