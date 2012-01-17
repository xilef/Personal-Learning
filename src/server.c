#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

/* 
 * Packet received will have a maximum of 256 bytes
 * Packet format is:
 * 	cmd (1 byte)
 * 	len (1 bytes)
 * 	string (254 bytes including null terminator)
 */
#define MAXDATASIZE 256
#define CMD 0
#define LEN 1

void acceptconn(int sockfd);
void handle_message(char *msg);
int send_all(const int sockfd, const char *buff, int *len);
int recv_all(const int sockfd, char *buff, int *len);
void *get_in_addr(struct sockaddr *sa);

int main(int argc, char *argv[])
{
	if (argc < 3 || argv [1][1] != 'p') {
		printf("Usage:\n");
		printf("SmartEchoServer -p <port>\n");
		exit(1);
	}
	signal(SIGCHLD, SIG_IGN);
	char port[6];

	pid_t pid;
	int sockfd;				// socket fd
	int status;				// socket return
	int yes = 1;				// for setsockopt
	struct addrinfo hints, *res, *p;

	int new_fd;
	socklen_t sin_size;			// sockaddr_storage size
	struct sockaddr_storage client_addr;
	char client[INET6_ADDRSTRLEN];

	// Initialize
	strcpy(port, argv [2]);
	memset(&hints, 0, sizeof (hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(NULL, port, &hints, &res)) != 0) {
		fprintf(stderr, "server: getaddrinfo: %s\n", gai_strerror(status));
		return 2;
	}

	// Create a socket
	for (p = res; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		// Port reuse
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (int)) == -1) {
			close(sockfd);
			perror("server: setsockopt");
			continue;
		}

		if  (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	// No socket created
	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	freeaddrinfo(res);

	// Start to listen on the socket
	if (listen(sockfd, 10) == -1) {
		perror("server: listen");
		exit(1);
	}

	printf("server: waiting for connections\n");

	// Main server loop
	while (1) {
		// Try to accept incoming connection
		sin_size = sizeof (client_addr);
		new_fd = accept(sockfd, (struct sockaddr *) &client_addr, &sin_size);

		// Error accepting connection
		if (new_fd == -1) {
			perror("server: accept");
			continue;
		}

		inet_ntop(client_addr.ss_family, get_in_addr ((struct sockaddr *) &client_addr), client, sizeof (client));
		printf("server: %s connected!\n", client);

		pid = fork();
		if (pid == -1) {
			perror("fork");
			close(new_fd);
			continue;
		} else if(pid == 0) { // Child process
			close(sockfd);	// No need for main socket
			printf("server: process message for %s [%d]\n", client, new_fd);
			acceptconn(new_fd);
			printf("server: closing connection to %s [%d]\n", client, new_fd);
			close(new_fd);
			exit(0);
		} else { // Parent process
			close(new_fd);
			while(waitpid (-1, NULL, WNOHANG) > 0);
		}
	}
	return 0;
}

void acceptconn(int new_fd)
{
	int sendbyte, recvbyte;
	char message[MAXDATASIZE];

	// Erase the buffer for the packet being sent by the client
	memset(message, 0, sizeof (message));
	if (recv_all(new_fd, message, &recvbyte) == -1) {
		perror("server: recv");
		close(new_fd);
		return;
	}

	printf("server: %d bytes received\n", recvbyte);
	handle_message(message);

	// Return the new packet to the client
	sendbyte = strlen(message);
	if (send_all(new_fd, message, &sendbyte) == -1) {
		perror("send");
		close(new_fd);
		return;
	}

	printf("server: %d bytes sent\n", sendbyte);
}

void handle_message(char *msg)
{
	int cmd = msg[CMD];
	int len = msg[LEN];
	int i;

	strcpy(msg, msg + LEN);

	switch (cmd) {
	case 0x01: { // Increment ASCII value of each char of the string
		for (i = 1; i <= len; i++)
			msg[i]++;
		break;
	}
	case 0x02: { // Decrement ASCII value of each char of the string
		for (i = 1; i <= len; i++)
			msg[i]--;
		break;
	}
	case 0x03: { // Capitalize all leters of the string
		for (i = 1; i <= len; i++) {
			msg[i] = toupper(msg [i]);
		}
		break;
	}
	default:
		break;
	}
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
			return(temp);
		total += temp;
		byteleft -= temp;
	}

	*len = total;
	return(0);
}

// Helper function to take care of receiving all bytes intended to be sent by the client
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

		plen = buff[LEN];
		stlen = strlen(buff + LEN);
		total += temp;
		recvlen = plen - stlen;
	} while (stlen < plen);			// If the length of the string received does not match
									// the length stated in the packet, keep on receiving.

	*len = total;
	return(0);
}

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
