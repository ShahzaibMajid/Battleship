/* BattleShip 2.0 (because it takes two to play)
   Authors: Matthew Sanchez and Shahzaib Majid
   CSC524 Computer Networks
   TCP SERVER CODE
   Designed for Linux OS
   Cited source: http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
*/

/* Libraries To Include */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

/* Definitions */
#define PORT "3369" //port that is used for server and players
#define MAX_PLAYERS 2 //max num of players
typedef struct {

} Grid;
int players[MAX_PLAYERS];

/* Grid Generator*/
void generateGrid() {
}

/* Update Grid*/
void updateGrid() {
}

/* Main Controller*/
int main(int argc, char *argv[]) {

/* Variables To Include*/
  int sockfd; //socket file descriptor
  int status;
  struct sockaddr_storage player_addr;
  int player; //file descriptors for both players (essentially newfd)
  int how; //use this to prevent a player from sending after his turn
  struct addrinfo hints, *res;
  struct addrinfo *servinfo;
  socklen_t addr_size;
  int yes = 1;
  int playersNeeded = 2;
  int x = 0; //x coordinate in grid
  int y = 0; //y coordinate in grid

/* Establish Server*/
  memset(&hints,0,sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM; //TCP socket type
  hints.ai_flags = AI_PASSIVE; //my IP is used

  if((status = getaddrinfo(NULL,PORT,&hints,&servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    exit(1); //Good Job You Failed :D
  }

  /* Wait to accept incoming connection*/
  // loop through all results and bind to the first we can
  for(res = servinfo; res != NULL; res = res->ai_next) {
    if((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }

    if(setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      perror("setsockopt");
      exit(1);
    }

    if(bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
      close(sockfd);
      perror("server : bind");
      continue;
    }
    break;
  }

  if(res == NULL) {
    fprintf(stderr, "server: failed to bind\n");
    return 2; //Good Job You Failed :D
  }

  freeaddrinfo(servinfo); //all done with servinfo

  /*Server Established */

  /*Begin Queuing Players for Game*/
  if(listen(sockfd,MAX_PLAYERS) == -1) {
    perror("listen");
    exit(1);
  }
  printf("Battleship Server : Waiting for %d Player(s) to Connect...\n",playersNeeded);

  /*Get Players*/
  while(playersNeeded != 0) {
    addr_size = sizeof(player_addr);
    player = accept(sockfd,(struct sockaddr *)&player_addr, &addr_size);
    if (player == -1) {
      perror("Error in Connecting Player to Server");
      continue;
    } else {
      printf("Battleship Server : Waiting for %d Player(s) to Connect...\n",--playersNeeded);
    }
  }
  printf("All players connected.\n");
  printf("Server is generating Battlefield grids for both players...\n");

  //---Generate the grids for both players on server
  generateGrid();
  //---Ask players to place ships on grid as chosen

	return 0;
}
