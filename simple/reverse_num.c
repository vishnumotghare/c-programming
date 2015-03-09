#include <stdio.h>

int main()
{
	int num ,rem,res = 0;
	int temp;
	printf("Enter number\n");
	scanf("%d",&num);
	temp = num;
	
	while (num > 0) {
		rem = num % 10;
		res = (res * 10) + rem;
		num = num / 10;
	}
	
	printf("Number after reversing = %d\n",res);

	if ( temp == res)
		printf("%d is palondrome\n",temp);
	else
		printf("%d is not palindrome\n",temp);

	return 0;
}
