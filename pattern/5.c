#include <stdio.h>


int main()
{
	int i,j,n,t,k;
	
	printf("Enter NUmber of coloumn\n");
	scanf("%d",&n);

	for (i = 0; i < n; i++) {
		t = i;
		for (j = n; j > i; j--) {
			printf("*");
		}
		while (t > 0) {
			printf("  ");
			t--;
		}	

		for (k = n; k > i; k--) {
			printf("*");
		}
		printf("\n");
	}

#if 0
	for (i = 0; i < n; i++) {
		t = i;
		for (j = n; j > i; j--) {
			while (t > 0) {
				printf(" ");
				t--;
			}	
			printf("*");
		}
		printf("\n");
	}
	 for (i = 0; i < n; i++) {
		t = n-i-1;
                for (j = 0; j <= i; j++) {
			while (t > 0) {
				printf(" ");
				t--;
			}
                        printf("*");
                }
                printf("\n");
        }

	
#endif

	return 0;
}

