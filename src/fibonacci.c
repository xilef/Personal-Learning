/*
 * Simple fibonacci program
 * 	output up to nth sequence
 */

#include <stdio.h>

void fibonacci(const int num);

int main()
{
	int num;

	printf("Enter number: ");
	scanf("%d", &num);
	fibonacci(num);
	return 0;
}

void fibonacci(const int num)
{
	int num1, num2, temp, ctr;
	num1 = 0;
	num2 = 1;
	temp = 0;
	ctr = 2;

	if (num >= 1) {
		printf("%d ", num1);
	} if (num >= 2) {
		printf("%d ", num2);
	} if (num > 2) {
		while (ctr < num) {
			temp = num1 + num2;
			num1 = num2;
			num2 = temp;
			printf("%d ", temp);
			ctr++;
		}
	}
	printf("\n");
}
