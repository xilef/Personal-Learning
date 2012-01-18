/*
 * Own implementation of strstr
 */

#include <stdio.h>
#include <string.h>

#define MAX_SIZE 200

const char *strstr2(const char *str1, const char *str2);

int main()
{
	char str1[MAX_SIZE];
	char str2[MAX_SIZE];
	const char *res;

	printf("Enter string: ");
	fgets(str2, MAX_SIZE - 1, stdin);	
	printf("Enter string to find: ");
	fgets(str1, MAX_SIZE - 1, stdin);

	res = strstr2(str1, str2);

	if (res == NULL)
		printf("Not found\n");
	else
		printf("%s\n", res);

	return 0;
}

const char *strstr2(const char *str1, const char *str2)
{
	int x, y, len1, len2;
	const char *start;

	len1 = strlen(str1);
	len2 = strlen(str2);
	x = 0;
	start = NULL;

	for (y = 0; y < len2; y++) {
		if (str1[x] == str2[y]) {
			if (start == NULL)
				start = str2 + y;
			x++;
		} else {
			x = 0;
			start = NULL;
		}
		
		if (x == len1 - 1)
			return start;
	}
	
	return NULL;
}
