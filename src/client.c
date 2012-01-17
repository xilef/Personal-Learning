#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

/* 
 * Packet received will have a maximum of 256 bytes
 * Packet format is:
 * 	cmd (1 byte)
 *  len (1 bytes)
 *  string (254 bytes including null terminator)
 */
#define MAXDATASIZE 256
#define CMD 0
#define SEND_LEN 1
#define RECV_LEN 0

int send_all(const int sockfd, const char *buff, int *len);
int recv_all(const int sockfd, char *buff, int *len);

int main(int argc, char *argv [])
{
	if (argc < 9) {
		printf("Usage:\n");
		printf("SmartEchoClient -p <port> -d <ip> -c <cmd> -s <string>\n");
		printf("String can only contain %d characters.\n", MAXDATASIZE - 2);
		exit(1);
	}

	int i;
	char ipadd[16];
	char port[6];
	int cmd;
	char string[MAXDATASIZE - SEND_LEN];
	char buff[MAXDATASIZE];

	int sockfd;			// socket fd
	int status;			// socket return
	struct addrinfo hints, *res, *p;
	int recvbyte, sendbyte;

	// Initialize vars
	for (i = 1; i < argc; i = i + 2) {
		switch (argv[i][1]) {
			case 'p': {
				strcpy(port, argv [i + 1]);
				break;
			}
			case 'd': {
				strcpy(ipadd, argv [i + 1]);
				break;
			}
			case 'c': {
				cmd = atoi(argv [i + 1]);
				break;
			}
			case 's': {
				strcpy(string, argv [i + 1]);
				break;
			}
			default:
				break;
		}
	}
	memset(buff, 0, sizeof (buff));
	memset(&hints, 0, sizeof (hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(ipadd, port, &hints, &res)) != 0) {
		fprintf (stderr, "getaddrinfo: %s\n", gai_strerror(status));
		exit(2);
	}

	// Create a socket
	for (p = res; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}
		
		break;
	}

	// No socket created
	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		exit (2);
	}

	// Create the packet
	buff[CMD] = cmd;
	buff[SEND_LEN] = strlen (string);
	strcpy(buff + SEND_LEN + 1, string);

	// Send packet to the server
	sendbyte = strlen(buff);
	if (send_all(sockfd, buff, &sendbyte) == -1) {
		perror("client: send");
		close(sockfd);
		exit(1);
	}

	// Erase the buffer for the new string returned by the server
	memset(buff, 0, sizeof (buff));
	if (recv_all(sockfd, buff, &recvbyte) == -1) {
		perror("client: recv");
		close(sockfd);
		exit(1);
	}

	// Output new string provided by the server
	printf("New string: %s\n", buff + RECV_LEN + 1);
	
	close(sockfd);
	return(0);
}

// Helper function to take care of sending all bytes in case of problem
int send_all(const int sockfd, const char *buff, int *len)
{
	int total = 0;
	int byteleft = *len;
	int temp = -1;

	while (total < *len) {			// If the sent byte does not equal the actual byte, 
									// keep on sending the remaining
		temp = send(sockfd, buff + total, byteleft, 0);
		if (temp == -1)
			return (temp);
		total += temp;
		byteleft -= temp;
	}

	*len = total;
	return(0);
}

// Helper function to take care of receiving all bytes intended to be sent by the server
int recv_all(const int sockfd, char *buff, int *len)
{
	int total = 0;
	int recvlen = MAXDATASIZE;
	int stlen;
	int plen;
	int temp;

	do {
		temp = recv(sockfd, buff + total, recvlen, 0);
		
		if (temp == -1 || temp == 0)
			return -1;
		
		plen = buff[RECV_LEN];
		stlen = strlen(buff + RECV_LEN + 1);
		total += temp;
		recvlen = plen - stlen;
	} while (stlen < plen);			// If the length of the string received does not match
									// the length stated in the packet, keep on receiving.

	*len = total;
	return (0);
}
