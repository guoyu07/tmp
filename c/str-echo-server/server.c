#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>

#define MAXLINE 1000

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

void sig_chld(int signo) {
    pid_t pid;
    int stat;

    pid = wait(&stat);
    printf("pid:%d child-pid:%d terminated\n",getpid(), pid);
}

void str_echo(int sockfd) {
    ssize_t n;
    char buf[MAXLINE];

again:
    while ((n=read(sockfd, buf, MAXLINE)) >0) {
        write(sockfd, buf, n);
    }

    if (n<0 && errno == EINTR)
        goto again;
    else {
        printf("str_echo: read error.\n");
        exit(0);
    } 
        
}

int main() {

    int re, sockaddr_size, clientaddr_size, connfd;
    struct sockaddr_in serveraddr, clientaddr;
    const struct sockaddr *p_sockaddr;

    signal(SIGCHLD, sig_chld);
    
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
    
    re = listen(sfd, 511);
    printf("listen result: %d\n", re);


    while (1) {
        printf("\nbegin accept\n");
        connfd = accept(sfd, (struct sockaddr *)&clientaddr, &clientaddr_size);
        printf("accept result: %d\n", connfd);
        print_sockaddr(&clientaddr); 
        if ( fork() == 0 ) {
            printf("chld: %d\n", getpid());
            close(sfd);
            str_echo(connfd);
            close(connfd);
        }
        close(connfd);
    }
}
