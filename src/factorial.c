/*
 * Simple factorial program
 */

#include <stdio.h>

int factorial(int num);

int main()
{
	int num;

	printf("Enter number: ");
	scanf("%d", &num);
	printf("Factorial is: %d\n", factorial(num));
	return 0;
}

int factorial(int num)
{
	int fact = 1;
	
	for (; num > 0; num--)
		fact *= num;
		
	return fact;
}
