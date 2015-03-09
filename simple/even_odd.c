#include <stdio.h>

int main()
{
	int num;
	
	printf("Enter number\n");
	scanf("%d",&num);
	
	if ((num & 1) == 0)
		printf("%d is even number\n",num);
	else	
		printf("%d is odd number\n",num);
	
	return 0;
}
