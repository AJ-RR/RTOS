
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "include/message.h"
#include "include/util.h"

#define MAX_CLIENTS 80
#define PORT 8080
#define SA struct sockaddr
#define BUFSIZE 1024

int sockfd, connfd;
char write_buffer[BUFSIZE] = {0};
char name[100];

void close_isr(int signum) {
	if(signum == SIGINT) {
		printf("Closed the connection\n");
		close(sockfd);
		exit(0);
	}
}

void send_chat(int sockfd)
{
		pa_simple *s = NULL;
    int ret = 1;
    int error;
    /* Create a new playback stream */
    if (!(s = pa_simple_new(NULL, name, PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__ ": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }
		for (;;) {
        uint8_t buf[BUFSIZE];
		struct Message *message = (struct Message *)malloc(sizeof(struct Message));
        ssize_t r;
        /* Read some data ... */
        if ((r = read(sockfd, message, sizeof(*message))) <= 0) {
            if (r == 0) /* EOF */
                break;

            fprintf(stderr, __FILE__": read() failed: %s\n", strerror(errno));
            goto finish;
        }
        /* ... and play it */
        if (pa_simple_write(s, message->msg, sizeof(message->msg), &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
            goto finish;
        }
    }
    /* Make sure that every single sample was played */
    if (pa_simple_drain(s, &error) < 0) {
        fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
        goto finish;
    }
    ret = 0;
		finish:
    if (s)
        pa_simple_free(s);

		return NULL;

}

void receive_chat(int sockfd){
		pa_simple *s = NULL;
    int ret = 1;
    int error;
    /* Create the recording stream */
    if (!(s = pa_simple_new(NULL, name, PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }
    for (;;) {
		struct Message *message = (struct Message *)malloc(sizeof(struct Message));
        uint8_t buf[BUFSIZE];
        /* Record some data ... */
        if (pa_simple_read(s, message->msg, sizeof(message->msg), &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            goto finish;
        }

        /* And write it to STDOUT */
        if (write(sockfd, message, sizeof(*message)) != sizeof(*message)) {
            fprintf(stderr, __FILE__": write() failed: %s\n", strerror(errno));
            goto finish;
        }
    }
    ret = 0;
		finish:
    if (s)
		pa_simple_free(s);

}

int main()
{

    struct sockaddr_in servaddr, cli;

    // Create and verify the socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign port and ip
    servaddr.sin_family = AF_INET;
    // servaddr.sin_addr.s_addr = inet_addr("40.70.68.58");
		servaddr.sin_addr.s_addr = inet_addr("40.121.60.204");
		// servaddr.sin_addr.s_addr = inet_addr("52.149.151.135");
		servaddr.sin_port = htons(PORT);

    // connect to client
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    if(fork() == 0){
      send_chat(sockfd);
    }
    else{
      receive_chat(sockfd);
    }

    close(sockfd);
}
