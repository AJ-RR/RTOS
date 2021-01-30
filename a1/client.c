
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

void send_chat(int sockfd)
{

  bzero(&sendBuffer,sizeof(sendBuffer));
  printf("\nType a message here ...  ");
  /*This function is used to read from server*/
  fgets(sendBuffer,10000,stdin);
  /*Send the message to server*/
  send(sockfd,sendBuffer,strlen(sendBuffer)+1,0);
  printf("\nMessage sent !\n");
    // char buff[MAX];
    // int n;
    // for (;;) {
    //     bzero(buff, sizeof(buff));
    //     printf("Enter the message : ");
    //     n = 0;
    //     while ((buff[n++] = getchar()) != '\n'); //check for enter key
    //     write(sockfd, buff, sizeof(buff)); //write to the socket
    //
    //     //exit case
    //     if ((strncmp(buff, "exit", 4)) == 0) {
    //         printf("Client Exit...\n");
    //         break;
    //     }
    // }
}

void receive_chat(int sockfd){

  bzero(&recvBuffer,sizeof(recvBuffer));
  /*Receive the message from server*/
  recv(sockfd,recvBuffer,sizeof(recvBuffer),0);
  printf("\nSERVER : %s\n",recvBuffer);
  // char buff[MAX];
  // int n;
  // for(;;){
  //   bzero(buff, sizeof(buff));
  //   n = 0;
  //   if(read(sockfd, buff, sizeof(buff)) != 0) //read from the socket
  //     printf("Server says : %s", buff);
  //
  //   //exit case
  //   if ((strncmp(buff, "exit", 4)) == 0) {
  //       printf("Client Exit...\n");
  //       break;
  //   }
  //
  // }
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
      send_chat(sockfd);
    }
    else{
      //full_duplex chat
      receive_chat(sockfd);
    }

    // close the socket
    close(sockfd);
}
