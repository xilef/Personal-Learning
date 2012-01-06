#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 2000
#define MAX_QUERY 500
#define MAX_STRING 50

char **substr (char **str, int num);

int main () {
	FILE *in;
	char buff[BUFFER_SIZE];
	char **str;
	char **strbuff;
	int k[MAX_QUERY];
	int n, q, x;
	int len;

	in = fopen ("input.txt", "r");
	if (in == NULL) {
		perror ("Failed to open file input.txt\n");
		return (EXIT_FAILURE);
	}

	fgets (buff, BUFFER_SIZE, in);
	n = atoi (buff);
	if (n > MAX_STRING) {
		fprintf (stderr, "Number of strings exceeds  MAX_STRING\n");
		return (EXIT_FAILURE);
	}

	str = (char **) malloc (MAX_STRING * sizeof (char *));
	for (x = 0; x < n; x++) {
		str[x] = (char *) malloc (MAX_STRING * sizeof (char *));
		fgets (buff, BUFFER_SIZE, in);
		len = strlen (buff);
		if (buff[len - 1] == '\n')
			buff[len - 1] = '\0';
		strcpy (str[x], buff);
	}

	fgets (buff, BUFFER_SIZE, in);
	q = atoi (buff);
	if (q > MAX_QUERY) {
		fprintf (stderr, "Number of queries exceeds MAX_QUERY\n");
		return (EXIT_FAILURE);
	}

	for (x = 0; x < q; x++) {
		fgets (buff, BUFFER_SIZE, in);
		k[x] = atoi (buff);
	}

	strbuff = substr (str, n);

	fclose (in);
	return (EXIT_SUCCESS);
}

char **substr (char **str, int num) {
	int start, end;
	int len, maxlen;
	char **buff;
	int x, y;

	maxlen = 0;
	start = 0;
	end = 1;
	y = 0;

	for (x = 0; x < num; x++) {
		len = strlen (str[x]);
		maxlen += (len * (len + 1)) / 2;
	}

	buff = (char **) malloc (maxlen * sizeof (char *));
	for (x = 0; x < maxlen; x++) {
		buff[x] = (char *) malloc (MAX_STRING * sizeof (char *));
		len = strlen (str[y]);

		memset (buff[x], 0, MAX_STRING);
		strncpy (buff[x], str[y] + start, end);
		end++;

		if (end + start > len) {
			start++;
			end = 1;
		}

		if (start == len) {
			start = 0;
			end = 1;
			y++;
		}
	}

	return (buff);
}
