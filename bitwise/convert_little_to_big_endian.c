#include <stdio.h>

int main()
{
	unsigned int num;
	unsigned int b0,b1,b2,b3;

	char * ptr = (char*)&num;

	printf("Enter number\n");
	scanf("%x",&num);

	b0 = (num & 0x000000FF) << 24;
	b1 = (num & 0x0000FF00) << 8;
        b2 = (num & 0xFF000000) >> 24;
        b3 = (num & 0x00FF0000) >> 8;

	num = b0 | b1 | b2 | b3;

	printf("num = %x\n",num);	

	return 0;	

}
