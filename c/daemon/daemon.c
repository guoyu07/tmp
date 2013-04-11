#include <stdio.h>
#include <signal.h>
#include <syslog.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
    
    pid_t pid;

    pid=fork();
    if (pid == 0) {
     
    } else if (pid > 0) {
        argv[0][0] = '1';
        sleep(10);
        exit(0);
    } else {
        return -1;
    }

    setsid();
    signal(SIGHUP, SIG_IGN);
    pid=fork();
    if (pid > 0 ) {
        argv[0][0] = '2';
        sleep(10);
        exit(0);
    }

    chdir("/");
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    sleep(10);
    return 0;
}

