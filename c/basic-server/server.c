#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

void print_sockaddr(const struct sockaddr_in *sockaddr) {
    
    printf("sin_len=%d\n", sockaddr->sin_len);
    printf("sin_family=");
    switch (sockaddr->sin_family) {
        case AF_INET:
            printf("AF_INET");
            break;

        case AF_INET6:
            printf("AF_INET6");
            break;

        default:
            printf("unkowned");
    }
    printf("\n");

    printf("sin_port=%d\n", ntohs(sockaddr->sin_port));
    printf("sin_addr=%d, %s\n", ntohl(sockaddr->sin_addr.s_addr), inet_ntoa(sockaddr->sin_addr));

}

int main() {

    int re, sockaddr_size, clientaddr_size, connfd;
    struct sockaddr_in serveraddr, clientaddr;
    const struct sockaddr *p_sockaddr;

    p_sockaddr = (const struct sockaddr *)&serveraddr;
    sockaddr_size = sizeof(struct sockaddr_in);

    bzero((void *)&serveraddr, sockaddr_size);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(8888);


    int sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    printf("sockfd: %d\n", sfd);

    re = bind(sfd, p_sockaddr, sockaddr_size);
    printf("bind result: %d\n", re);
    
    re = listen(sfd, 51111);
    printf("listen result: %d\n", re);

    char timebuf[1000];
    time_t ticks;
    pid_t pid;
    while (1) {
        printf("\n\n\nbegin accept\n");
        connfd = accept(sfd, (struct sockaddr *)&clientaddr, &clientaddr_size);
        printf("accept result: %d\n", connfd);
        print_sockaddr(&clientaddr);
        
        if ( (pid=fork()) == 0 ) {
            ticks = time(NULL);
            snprintf(timebuf, 1000, "pid=%d\n%.24s\n", getpid(), ctime(&ticks));
            write(connfd, timebuf, strlen(timebuf));
            printf("\0");
            sleep(10);
            close(connfd);
        }
        close(connfd);
    }
}
