
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

#define MAX 100
#define PORT 8080

#define SA struct sockaddr

char recvBuffer[1000],sendBuffer[1000];

void send_chat(int sockfd)
{
    // int n;

    bzero(sendBuffer, sizeof(sendBuffer));
    printf("Enter the message : ");
    // n = 0;
    // while ((sendBuffer[n++] = getchar()) != '\n'); //check for enter key
    gets(sendBuffer);
    write(sockfd, sendBuffer, sizeof(sendBuffer)); //write to the socket

    //exit case
    if ((strncmp(sendBuffer, "exit", 4)) == 0) {
        printf("Client Exit...\n");
        exit(0);
    }

}

void receive_chat(int sockfd){

  int n;

  bzero(recvBuffer, sizeof(recvBuffer));
  n = 0;
  if(read(sockfd, recvBuffer, sizeof(recvBuffer)) != 0) //read from the socket
    // printf("Server says : %s", recvBuffer);
    puts(recvBuffer);

  //exit case
  if ((strncmp(recvBuffer, "exit", 4)) == 0) {
      printf("Client Exit...\n");
      exit(0);
  }


}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and varification
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
      perror("Failed to create the socket\n");
    }
    // if (sockfd == -1) {
    //     printf("socket creation failed...\n");
    //     exit(0);
    // }
    else
        printf("Successfully created the socket\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign the port and ip
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("40.70.68.58");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection with the server failed\n");
        exit(0);
    }
    else
        printf("Connected to the server\n");

    if(!fork()){
      // half_duplex chat
      while(1){
        send_chat(sockfd);
      }

    }
    else{
      //full_duplex chat
      while(1){
        receive_chat(sockfd);
      }

    }

    // close the socket
    close(sockfd);
}
