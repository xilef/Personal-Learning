/*
 * Simple calculator program
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int num1, num2;
	char op;

	if (argc < 4) {
		printf("Usage is:\n");
		printf("calculator <num1> <operator> <num2>\n");
		printf("Operators accepted are: + - / *\n");
	}

	num1 = atoi(argv[1]);
	op = argv[2][0];
	num2 = atoi(argv[3]);

	printf("Result: ");
	switch (op) {
	case '+':
		printf("%d", num1 + num2);
		break;
	case '-':
		printf("%d", num1 - num2);
		break;
	case '*':
		printf("%d", num1 * num2);
		break;
	case '/':
		printf("%d", num1 / num2);
		break;
	default:
		printf("Invalid operator used");
	}
	printf("\n");
	return 0;
}
