#include <stdio.h>

int main()
{
	int a, b, c;

	printf("Enter number\n");
	scanf("%d %d %d",&a,&b,&c);
	
	if (a > b && a > c)
		printf("%d is bigger than %d and %d\n",a,b,c);
	if (b > a && b > c)
		printf("%d is bigger than %d and %d\n",b,a,c);
	if (c > a && c > b)
		printf("%d is bigger than %d and %d\n",c,a,b);
}
