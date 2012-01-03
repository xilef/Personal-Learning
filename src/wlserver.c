#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

const int MAX_SLEEP = 120;

void terminate (int signum)
{
	int i;
	srand (time (NULL));
	i = rand () % 10 + 1;
	
	if (i < 9) {
		printf ("Terminated\n");
		exit (1);
	}
}

void start_server (char *serv)
{
	int i;
	FILE *out;
	char fname[60] = "/prod/applc/wls/crrs/logs/system/";
	char buff[50];
	
	struct tm *loctime;
	time_t curtime;

	curtime = time (NULL);
	loctime = localtime (&curtime);

	strcat (fname, serv);
	strcat (fname, "server/crrs");
	strcat (fname, serv);
	strcat (fname, "console.log");
	
	srand (time (NULL));
	i = rand () % 10 + 1;

	out = fopen (fname, "w");
	fprintf (out, "\n");
	strftime (buff, 50, "<%b %d, %Y %I:%M:%S %p %Z>", loctime);

	if (i < 2) {
                fprintf (out, "%s <Notice> <WebLogicServer <BEA-000365> ", buff);
                fprintf (out, "<Server failed to start>\n");
                fclose (out);
                exit (2);
        }

	i = rand () % MAX_SLEEP + 1;
	sleep (i);
	
	fprintf (out, "%s <Notice> <WebLogicServer> <BEA-000365> ", buff);
	fprintf (out, "<Server state changed to RUNNING>\n");
	fprintf (out, "%s <Notice> <WebLogicServer> <BEA-000360> ", buff);
	fprintf (out, "<Server started in RUNNING mode>\n");
	fclose (out);
}

int main (int argc, char *argv[])
{
	if (argc < 2) {
		printf ("Usage:\n");
		printf ("%s DcrrsM/DcrrsA\n", argv[0]);
		exit (1);
	}

	if (strcmp (argv[1], "DcrrsA") == 0)
		start_server ("A");
	else if (strcmp (argv[1], "DcrrsM") == 0)
		start_server ("M");
	else {
		printf ("Invalid argument!\n");
		exit (1);
	}

	signal (SIGTERM, terminate);

	while (1) {
	}

	return 0;
}
