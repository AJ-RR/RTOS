#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// chattion designed for chat between client and server.
void chat(int sockfd)
{
    char buff[MAX];
    int n;
    // infinite loop for chat
    for (;;) {
        bzero(buff, MAX);

        read(sockfd, buff, sizeof(buff));

        printf("Client: %s\n", buff);
        printf("Enter response: ");
        bzero(buff, MAX);
        n = 0;

        while ((buff[n++] = getchar()) != '\n');
        write(sockfd, buff, sizeof(buff));

        // exit condition
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}


int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // create and verify the socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
      printf("Failed to create socket\n");
      exit(0);
    }
    else
        printf("Successfully created the socket\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign port and ip
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding the socket to the ip
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Failed to bind the socket\n");
        exit(0);
    }
    else
        printf("Successfully in binding the socket\n");

    // server is ready to listen
    if ((listen(sockfd, 5)) != 0) {
        printf("Cannot listen anymore\n");
        exit(0);
    }
    else
        printf("Server listening\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("Failed to accept the client\n");
        exit(0);
    }
    else
        printf("Client accepted\n");

    // chat
    chat(connfd);

    close(sockfd);
}
