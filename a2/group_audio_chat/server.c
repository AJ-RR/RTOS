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
#include <pthread.h>

#include "include/message.h"
#include "include/util.h"

#define MAX_CLIENTS 80
#define PORT 8080
#define SA struct sockaddr

int sockfd; //server socketfd
int connfds[MAX_CLIENTS] = {0};
int curr_clients = 0;

pthread_mutex_t num_client_mutex;
pthread_t threads[MAX_CLIENTS]; //MAX_CLIENTSimum number of clients and threads

void close_isr(int signum) {
	if(signum == SIGINT) {
		printf("Closing server\n");
		for (int i = 0; i < curr_clients; i++) {
            pthread_kill(threads[i], SIGKILL);
        }
		for(int i = 0; i < curr_clients; i++) {
			close(connfds[i]);
		}
		close(sockfd);
		pthread_exit(NULL);
		exit(0);
	}
}

void receive_chat(int sockfd, int id){
  char buff[MAX_CLIENTS];
  int n, rd, num_clients = 0;
  for (;;) {
      bzero(buff, MAX_CLIENTS);

			struct Message *message = (struct Message *)malloc(sizeof(struct Message));

      if((rd = read(sockfd, message, sizeof(*message))) == -1){
        perror("Error reading from client\n");
      }

      if(rd != 0){
        for(int j=0; j<curr_clients; j++){
          if(connfds[j] != sockfd && connfds[j] !=0){
            char clientId[100];
            sprintf(clientId, "%d", id);
            strcat(clientId," says: ");
            strcat(clientId, buff);
            write(connfds[j], message, sizeof(*message));
          }
        }
      }
      else{
        int loc;
        for(int i = 0; i < curr_clients; i++) {
        	if(sockfd == connfds[i]) {
        		loc = i;
        	}
        }
        for(int i = loc; i < curr_clients; i++) {
        	connfds[i] = connfds[i+1];
        }
	      curr_clients--;
        break;
      }
  }
}

void register_client(int sockfd){
  char reg_id[100];
  sprintf(reg_id, "%d", curr_clients);
  strcat(reg_id, " is your ID");

  if((write(sockfd, reg_id, sizeof(reg_id))) == -1){
    perror("Could not register client\n");
  }
  else{
    printf("Client %d has been successfully registered\n", curr_clients);
  }

}

void * client_handler(void * arg){

  int newSocket = *((int*) arg);
	register_client(newSocket);
  receive_chat(newSocket, curr_clients);
}

int main()
{
    int connfd, len;
    struct sockaddr_in servaddr, cli;
		signal(SIGINT, close_isr);
    // create and verify socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
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

    int i = 0;
    while(1){
      len = sizeof(cli);

      //verify the client
      connfd = accept(sockfd, (SA *) &cli, &len);
      if (connfd < 0) {
          printf("Failed to accept client\n");
          exit(0);
      }
      else{
        connfds[i] = connfd;
        printf("Accepted the client %d with sockfd %d\n", curr_clients, connfd);
      }


      if(pthread_create(&threads[i++], NULL, client_handler, (void*) &connfd) < 0 )
            perror("Failed to create thread\n");

      pthread_mutex_lock(&num_client_mutex);
      curr_clients++;
      pthread_mutex_unlock(&num_client_mutex);
    }

    pthread_exit(NULL);
    close(sockfd);
}
