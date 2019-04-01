#include <stdio.h>

int main()

{
	unsigned int num = 0x1;

	char *ptr = (char*)&num;

	if (*ptr == 0x1)
		printf("Machine is little endian\n");
	else
		printf("Machine is big endian\n");


	return 0;
}
