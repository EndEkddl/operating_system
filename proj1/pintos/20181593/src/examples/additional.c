#include <stdio.h>
#include <syscall.h>
#include <stdlib.h>
//#include "../lib/user/syscall.h"

int main(int argc, char *argv[]){

	int a, b, c, d, max4, fibo;

	if(argc != 5) return EXIT_FAILURE;

	a = atoi(argv[1]);
	b = atoi(argv[2]);
	c = atoi(argv[3]);
	d = atoi(argv[4]);

	fibo = fibonacci(a);
	max4 = max_of_four_int(a,b,c,d);

	printf("%d %d\n", fibo, max4);

	return EXIT_SUCCESS;
}
