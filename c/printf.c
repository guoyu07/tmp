#include <stdio.h>

int
main() {
	int dec = 100;
	int hex = 0x1d;
	double dou = .1;

	printf("%d\t%#d\n", dec, dec);
	printf("%x\t%#x\n", hex, hex);
	printf("%f\t%#f\n", dou, dou);
}

/*
100	100
1d	0x1d
0.100000	0.100000
 */
