#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 200

int check(const char *str);

int main()
{
	char str[MAX_SIZE];
	
	printf("Enter string to check: ");
	fgets(str, MAX_SIZE - 1, stdin);
	
	if (check(str))
		printf("Not a palindrome\n");
	else
		printf("String is a palindrome\n");

	return 0;
}

int check(const char *str)
{
	int x, y;
	y = strlen(str) - 1;
	x = 0;
	
	while (x < y) {
		if (!isalpha(str[x]) && !isdigit(str[x])) {
			x++;
			continue;
		}
		if (!isalpha(str[y]) && !isdigit(str[y])) {
			y--;
			continue;
		}
		if (str[x] != str[y])
			return 1;
		x++;
		y--;
	}

	return 0;
}
