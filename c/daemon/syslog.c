#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>
#include <errno.h>

int main() {
    char *log = "I'm the message. www.walu.cc";
    int errno;
    
    openlog("Log-prefix: ", LOG_PID|LOG_PERROR, LOG_UUCP);
    errno = random()%50;
    syslog(LOG_ERR, "Here is the <11> log, %s: %m", "error");
    syslog(LOG_ERR|LOG_NEWS, "Here is the log, %s: %m", "error");
    closelog();
    return 0;
}
