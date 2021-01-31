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

// sendtion designed for chat between client and server.
void send_chat(int sockfd)
{
    char buff[MAX];
    int n;

    for (;;) {
        bzero(buff, MAX);
        printf("Enter message : \n");
        bzero(buff, MAX);
        n = 0;

        while ((buff[n++] = getchar()) != '\n');

        write(sockfd, buff, sizeof(buff));
    }
}

void receive_chat(int sockfd){
  char buff[MAX];
  int n, rd;
  for (;;) {
      bzero(buff, MAX);

      rd = read(sockfd, buff, sizeof(buff));

      if(rd != 0)
        printf("From client: %s\n ", buff);
  }
}


int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // create and verify socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Falied to create the socket\n");
        exit(0);
    }
    else
        printf("Successfully created the socket\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign port and ip
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding the socket
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Failed to bind socket\n");
        exit(0);
    }
    else
        printf("Successfull in binding the socket\n");

    // server listen
    if ((listen(sockfd, 5)) != 0) {
        printf("Failed to listen\n");
        exit(0);
    }
    else
        printf("Server listening.....\n");
    len = sizeof(cli);

    //verify the client
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("Failed to accept client\n");
        exit(0);
    }
    else
        printf("Accepted the client...\n");

    if(fork() == 0){
      send_chat(connfd);
    }
    else{
      receive_chat(connfd);
    }

    close(sockfd);
}
