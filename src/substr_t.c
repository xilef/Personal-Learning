#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define BUFFER_SIZE 2000
#define MAX_QUERY 500
#define MAX_STRING 50
#define MAX_THREADS 2

struct node {
	char str[BUFFER_SIZE];
	struct node *left;
	struct node *right;
};

struct thread_data {
	struct node *list;
	char *str;
};

void *substr(void *argv);
int insert(struct node **head, struct node *newNode);
void search(struct node *current, unsigned long long *ctr, unsigned long long k);
void clear(struct node *head);

int main()
{
	pthread_t threads[MAX_THREADS];
	struct thread_data data[MAX_THREADS];
	char buff[BUFFER_SIZE];
	struct node *list = NULL;
	char **str;
	unsigned long long k[MAX_QUERY];
	unsigned long long len;
	int n, q, x, y;

	fgets(buff, BUFFER_SIZE, stdin);
	n = atoi(buff);
	if (n < 1 || n > MAX_STRING) {
		fprintf(stderr, "Number of strings should be > 0 and < MAX_STRING\n");
		return (1);
	}

	str = (char **) malloc(n * sizeof (char *));
	if (str == NULL) {
		fprintf(stderr, "No memory allocated for str\n");
		return (1);
	}
	for (x = 0; x < n; x++) {
		str[x] = (char *) malloc(BUFFER_SIZE * sizeof (str[x]));
		if (str[x] == NULL) {
			fprintf(stderr, "No memory allocated for str[x]\n");
			return (1);
		}
		fgets(buff, BUFFER_SIZE - 1, stdin);
		len = strlen(buff);
		if (buff[len - 1] == '\n')
			buff[len - 1] = '\0';
		strcpy(str[x], buff);
	}

	fgets(buff, BUFFER_SIZE, stdin);
	q = atoi(buff);
	if (q < 1 || q > MAX_QUERY) {
		fprintf(stderr, "Number of queries should be > 0 and < MAX_QUERY\n");
		return (1);
	}

	for (x = 0; x < q; x++) {
		fgets(buff, BUFFER_SIZE, stdin);
		k[x] = atoi(buff);

		if (k[x] < 1 || k[x] > 1000000000)
			fprintf(stderr, "Query should be > 0 and < 1000000000\n");
	}

	y = 0;
	for (x = 0; x < n; x++) {
		data[y].str = str[x];
		data[y].list = NULL;
		if (y + 1 < MAX_THREADS)
			pthread_create(&threads[y], NULL, substr, (void *)&(data[y]));
		else
			substr((void *)&(data[y]));
		y++;
	}

	for (x = 0; x < MAX_THREADS; x++)
		pthread_join(threads[x], NULL);

	for (x = 0; x < q; x++) {
		len = 0;
		search(list, &len, k[x]);

		if (len < k[x])
			printf("INVALID\n");
	}

	clear(list);
	
	for (x = 0; x < n; x++)
		free(str[x]);
		
	free(str);

	return (0);
}

// Create all the possible substrings from the input
void *substr(void *argv)
{
	struct thread_data *arg;
	struct node **head = NULL;
	struct node *newNode;
	int start, end;
	int len;
	clock_t st, ed;
	double t;

	arg = (struct thread_data *)argv;
	head = &(arg->list);
	len = strlen(arg->str);

	st = clock();
	for (start = 0; start < len; start++) {
		for (end = start + 1; end <= len; end++) {
			newNode = (struct node *)malloc(sizeof (struct node));
			if (newNode == NULL) {
				fprintf(stderr, "No memory allocated for newNode\n");
				exit (1);
			}
			strncpy(newNode->str, arg->str + start, end);
			newNode->str[end - start] = '\0';
			newNode->left = NULL;
			newNode->right = NULL;

			if (insert(head, newNode) == 1)
				free(newNode);
		}
	}
	ed = clock();
	t = (double)(ed - st) / CLOCKS_PER_SEC;
	
	printf("It took %lf seconds.\n", t);
	
	pthread_exit(NULL);
}

// non-recursive insert into the binary tree
int insert(struct node **head, struct node *newNode)
{
	struct node *current;
	struct node *prev;
	int res;
	current = *head;

	while (current != NULL) {
		res = strcmp(newNode->str, current->str);
		prev = current;

		if (res == 0) {
			return(1);
		}
		else if (res < 0)
			current = current->left;
		else
			current = current->right;
	}

	if ((*head) == NULL) 
		(*head) = newNode;
	else if (current == NULL) {
		if (res < 0)
			prev->left = newNode;
		else
			prev->right = newNode;
	}

	return(0);
}

// Recursive search
void search(struct node *current, unsigned long long *ctr, unsigned long long k)
{
	if (current == NULL)
		return;

	if ((*ctr) < k) {
		search(current->left, ctr, k);
		(*ctr)++;
		if ((*ctr) == k) {
			printf("%s\n", current->str);
			return;
		}
		if ((*ctr) <= k)
			search(current->right, ctr, k);
	}
}

void clear(struct node *head)
{
	if (head == NULL)
		return;

	clear(head->left);
	clear(head->right);
	free(head);
}

