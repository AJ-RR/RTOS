
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
#define PORT 8090
#define SA struct sockaddr

void send_chat(int sockfd)
{
    char buff[MAX];
    int n;

    for (;;) {
        bzero(buff, MAX);
        // printf("Enter message : \n");
        bzero(buff, MAX);
        n = 0;

        while ((buff[n++] = getchar()) != '\n');

        write(sockfd, buff, sizeof(buff));
    }
}

void receive_chat(int sockfd){
  char buff[MAX];
  int n;
  int rd;
  for (;;) {
      bzero(buff, MAX);

      rd = read(sockfd, buff, sizeof(buff));

      if(rd != 0)
        printf("%s\n ", buff);
  }
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // Create and verify the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
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
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
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
