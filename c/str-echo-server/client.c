#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/select.h>

#include "../unp.h"


void str_cli(FILE *fp, int sockfd) {
    char sendline[MAXLINE], readline[MAXLINE];
    int n, maxfd;
    fd_set rset;

    while (1) {
        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfd=max(fileno(fp), sockfd) + 1;
        
        select(maxfd, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset)) {
            if (Readline(sockfd, readline, MAXLINE) == 0) {
                err_quit("str_cli: server terminated prematurely");
            }
            Fputs(readline, stdout);
        }

        if (FD_ISSET(fileno(fp), &rset)) {
            if (Fgets(sendline, MAXLINE, fp)==NULL) {
                return;
            }
            Writen(sockfd, sendline, strlen(sendline));
        }

    };

    /*
    while (Fgets(sendline, MAXLINE, fp) != NULL) {
        Writen(sockfd, sendline, strlen(sendline));
        if (Readline(sockfd, readline, MAXLINE) == 0) {
            err_quit("server terminated prematurely.");
        }
        Fputs(readline, stdout);
    }
    */

}


int main() {

    int re, sockaddr_size, clientaddr_size, connfd;
    struct sockaddr_in serveraddr, clientaddr;
    const struct sockaddr *p_sockaddr;

    p_sockaddr = (const struct sockaddr *)&serveraddr;
    sockaddr_size = sizeof(struct sockaddr_in);

    bzero((void *)&serveraddr, sockaddr_size);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8888);
    inet_aton("127.0.0.1", &(serveraddr.sin_addr.s_addr));
    printf("addr: %d\n", serveraddr.sin_addr.s_addr);

    int sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    printf("sockfd: %d\n", sfd);
   
    re = connect(sfd, (struct sockaddr *)&serveraddr, sockaddr_size);
    printf("connect: %d\n", re);


   str_cli(stdin, sfd);
}
