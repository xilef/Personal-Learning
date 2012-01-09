#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 2000
#define MAX_QUERY 500
#define MAX_STRING 50

struct node {
	char str[BUFFER_SIZE];
	struct node* left;
	struct node* right;
};

struct node* substr (char** str, int num);
void insert (struct node** current, struct node* newNode);
void search (struct node* current, int* ctr, int k);

int main () {
	char buff[BUFFER_SIZE];
	struct node* list = NULL;
	char** str;
	int k[MAX_QUERY];
	int n, q, x;
	int len, maxcount;

	fgets (buff, BUFFER_SIZE, stdin);
	n = atoi (buff);
	if (n < 1 || n > MAX_STRING) {
		fprintf (stderr, "Number of strings should be > 0 and < MAX_STRING\n");
		return (EXIT_FAILURE);
	}

	str = (char**) malloc (MAX_STRING * sizeof (char*));
	for (x = 0; x < n; x++) {
		str[x] = (char*) malloc (MAX_STRING * sizeof (char*));
		fgets (buff, BUFFER_SIZE, stdin);
		len = strlen (buff);
		if (buff[len - 1] == '\n')
			buff[len - 1] = '\0';
		strcpy (str[x], buff);
	}

	fgets (buff, BUFFER_SIZE, stdin);
	q = atoi (buff);
	if (q < 1 || q > MAX_QUERY) {
		fprintf (stderr, "Number of queries should be > 0 and < MAX_QUERY\n");
		return (EXIT_FAILURE);
	}

	for (x = 0; x < q; x++) {
		fgets (buff, BUFFER_SIZE, stdin);
		k[x] = atoi (buff);

		if (k[x] < 1 || k[x] > 1000000000) {
			fprintf (stderr, "Query should be > 0 and < 1000000000\n");
		}
	}

	maxcount = 0;
	for (x = 0; x < n; x++) {
		len = strlen (str[x]);
		maxcount += (len * (len + 1)) / 2;
	}

	list = substr (str, n);

	for (x = 0; x < q; x++) {
		len = 0;
		search (list, &len, k[x]);
		
		if (len < k[x])
			printf ("INVALID\n");
	}

	return (EXIT_SUCCESS);
}

// Create all the possible substrings from the input
struct node* substr (char** str, int num) {
	struct node* head = NULL;
	struct node* newNode = NULL;
	int start, end;
	int len;
	int x;

	start = 0;
	end = 1;
	x = 0;
	len = strlen (str[x]);

	// get the string pointed to from start to end
	// increment end by 1
	// increment start by 1 when the end pointer has reached the end of string
	while (1) {
		newNode = (struct node*) malloc (sizeof (struct node));
		strncpy (newNode->str, str[x] + start, end);
		newNode->left = NULL;
		newNode->right = NULL;

		insert (&head, newNode);
		end++;

		if (end + start > len) {
			start++;
			end = 1;
		}

		if (start == len) {
			start = 0;
			end = 1;
			x++;
			if (x == num)
				break;
			len = strlen (str[x]);
		}
	}

	return (head);
}

// Insert into the tree in alphabetical order
void insert (struct node** current, struct node* newNode) {
	if (*current == NULL)
		(*current) = newNode;
	else if (strcmp (newNode->str, (*current)->str) == 0)
		free (newNode);
	else if (strcmp (newNode->str, (*current)->str) < 0)
		insert (&((*current)->left), newNode);
	else if (strcmp (newNode->str, (*current)->str) > 0)
		insert (&((*current)->right), newNode);
}

void search (struct node* current, int* ctr, int k) {
	if (current == NULL)
		return;

	if ((*ctr) < k) {
		search (current->left, ctr, k);
		(*ctr)++;
		if ((*ctr) == k) {
			printf ("%s\n", current->str);
			return;
		}
		if ((*ctr) <= k)
			search (current->right, ctr, k);
	}
}
