#define MAX 80
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT 8080
#define SA struct sockaddr

char recvBuffer[1000],sendBuffer[1000];

// Function designed for chat between client and server.
// void func(int sockfd)
// {
//     char buff[MAX];
//     int n;
//     // infinite loop for chat
//     for (;;) {
//         bzero(buff, MAX);
//
//         // read the message from client and copy it in buffer
//         read(sockfd, buff, sizeof(buff));
//         // print buffer which contains the client contents
//         printf("From client: %s\t To client : ", buff);
//         bzero(buff, MAX);
//         n = 0;
//         // copy server message in the buffer
//         while ((buff[n++] = getchar()) != '\n')
//             ;
//
//         // and send that buffer to client
//         write(sockfd, buff, sizeof(buff));
//
//         // if msg contains "Exit" then server exit and chat ended.
//         if (strncmp("exit", buff, 4) == 0) {
//             printf("Server Exit...\n");
//             break;
//         }
//     }
// }


void send_chat(int sockfd)
{
    bzero(&sendBuffer,sizeof(sendBuffer));
    printf("\nType a message here ...  ");
    /*Read the message from client*/
    fgets(sendBuffer,10000,stdin);
    /*Sends the message to client*/
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
  /*Receiving the request from client*/
  recv(sockfd,recvBuffer,sizeof(recvBuffer),0);
  printf("\nCLIENT : %s\n",recvBuffer);
  // char buff[MAX];
  // int n;
  // for(;;){
  //   bzero(buff, sizeof(buff));
  //   n = 0;
  //   if (read(sockfd, buff, sizeof(buff)) != -1) //read from the socket
  //     printf("Client says : %s", buff);
  //
  //   //exit case
  //   if ((strncmp(buff, "exit", 4)) == 0) {
  //       printf("Server Exit...\n");
  //       break;
  //   }
  //
  // }
}
// Driver function
int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");

    if(!fork()){
      // half_duplex chat
      send_chat(sockfd);
    }
    else{
      //full_duplex chat
      receive_chat(sockfd);
    }
    // // Function for chatting between client and server
    // func(connfd);

    // After chatting close the socket
    close(sockfd);
}
