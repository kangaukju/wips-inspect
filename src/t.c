#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

int main()
{
	int a = 0x000217a3;
	printf("%d  %d\n", a, ntohl(a));
}
