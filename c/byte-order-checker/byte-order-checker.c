#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
union {
    short target;
    char c[sizeof(short)];
}un;

int byte_order_check() {
    un.target = 0x0102;
    if (un.c[0]==0x01 && un.c[1]==0x02) {
        return 2;
    } else if (un.c[0]==0x02 && un.c[1]==0x01) {
        return 1;
    } else {
        return -1;
    }
}

int main() {
    int i = byte_order_check();
    uint32_t test, tmp;
    switch (i) {
        case 1:
            printf("little-endian\n");
            break;

        case 2:
            printf("big-endian\n");
            break;

        default:
            printf("unknown byte order\n");
            break;
    }
    printf("sizeof(short)=%d\n", sizeof(short));


    test = htonl(1);
    if ((uint32_t)1 != ntohl(test)) {
        printf("error");
    }

    

}
