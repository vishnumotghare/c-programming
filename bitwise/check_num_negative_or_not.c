#include <stdio.h>

int main()
{
	int num;
	
	printf("Enter Number\n");
	scanf("%d",&num);

	if ( (num & 0x80000000 ) != 0 )
		printf("Number is -ve\n");
	else
		printf("Number is +ve\n");

	return 0;
}
