#include <stdio.h>

/* This program set the perticular bit in a given
 * Integer
 */

unsigned int set_bit(unsigned int num, int pos)
{
	return (num | 1 << pos);
}

int main()
{
	unsigned int num;
	unsigned ret;
	int pos;

	printf("Enter number:\n");
	scanf("%d",&num);
	printf("Enter position to set bit\n");
	scanf("%d",&pos);
	
	ret = set_bit(num,pos);

	printf("Value after setting %d bit = %d\n",pos,ret);

	return 0;
}
