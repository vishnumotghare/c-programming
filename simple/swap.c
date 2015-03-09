#include <stdio.h>

void swap (int *a, int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;	
}

int main()
{
	int a, b;
	
	printf("enter value of a nd b\n");

	scanf("%d %d",&a,&b);

	printf("Value before swap  a = %d and b = %d\n",a,b);

	swap(&a,&b);

	printf("Value after swap  a = %d and b = %d\n",a,b);

	return 0;
	
}
