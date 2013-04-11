#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

int main(int argc, char **argv) {
    char *ptr, **pptr;
    char str[INET_ADDRSTRLEN];

    struct hostent *hptr;
    struct in_addr addr;

    while (--argc > 0) {
        ptr = *++argv;

        if (1 != inet_pton(AF_INET, ptr, &addr)) {
            perror(strerror(errno));
            exit(1);
        }

        hptr = gethostbyaddr(&addr, sizeof(struct in_addr), AF_INET);
        if (NULL == hptr) {
            printf("%s\n", hstrerror(h_errno));
            continue;
        }

        printf("official hostname: %s\n", hptr->h_name);

        for (pptr = hptr->h_aliases; *pptr != NULL; pptr++) {
            printf("\talias: %s\n", *pptr);
        }

        switch (hptr->h_addrtype) {
            case AF_INET:
                pptr = hptr->h_addr_list;
                for (; *pptr != NULL; pptr++) {
                    printf("%s\n", *pptr, inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
                }
                break;
        }
    }
}
