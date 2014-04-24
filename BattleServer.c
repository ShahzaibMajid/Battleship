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
#define DIM 11 //dimension

int players[MAX_PLAYERS]; //store IP address of player  
/*Grid Stuff*/
typedef struct {
  char grid[DIM][DIM]; //11 x 11 grid (actually 10 x 10)
} Grid;
Grid pl_1; //grid of player 1
Grid pl_2; //grid of player 2
int grid_height = 11;
int grid_length = 11;
char t_por[7]; //top portion of the box
char m_por[7]; //middle portion of the box

/*Grid Stuff Completed*/


/*Print out Grid*/
void printGrid() {

  int i,j = 0;
 
  for(j = 0; j < grid_height; ++j) {
    for(i = 0; i < grid_width; ++i) {
      printf("%s",t_por);
      if(i == (grid_width-1)) {
	printf("+");
      }
    }
    printf("\n");
    for(i = 0; i < grid_width;++i) {
      printf("%s",m_por);
      if(i == (grid_width-1)) {
	printf("|");
      }
    }
    printf("\n");
    for(i = 0; i < gridwidth; ++i) {
      printf("%s",m_por);
      if(i == (grid_width -1)) {
	printf("|");
      }
    }
    printf("\n");
    if (j == (grid_height-1)) {
      for (i=0; i<grid_width; ++i) {
	printf("%s", t_por);
	if (i == (grid_width - 1)) {
	  printf("+");
	}
      }
    }
  }
  printf("\n");

 /*
  for(i=0;i<DIM;i++) {
    for(j=0;j<DIM;j++) {
      printf("pl_1.grid[%d][%d] = %c\n",i,j,pl_1.grid[i][j]);
    }
  }
  */

}

/* Update Grid*/
void updateGrid() {

}
/* Grid Generator*/
void generateGrid(int x, int y) {

  int ascii = 65; //ascii value of 'A'
  int pos = 48; //first position on grid for row

  x = 0;
  pl_1.grid[0][0] = '#'; //marks no man land coordinate in grid                                  
  pl_2.grid[0][0] = '#';

  //Generate Letters A-L on columns of the grid
  for(y = 1;y < DIM; y++) {
    pl_1.grid[x][y] = (char)ascii;
    pl_2.grid[x][y] = (char)(ascii++);
  }  

  y = 0;
  //Set numbers on rows of the grid (#1-10)
  for(x = 1; x < DIM; x++) {
    pl_1.grid[x][y] =  (char)pos;
    pl_2.grid[x][y] =  (char)(pos++);
  }

  //Set battlefield with (W)ater character
  for(x = 1; x < DIM; x++) {
    for(y = 1; y < DIM; y++) {
      pl_1.grid[x][y] = 'W'; //W stands for Water
      pl_2.grid[x][y] = 'W';
    }
  }
  printGrid();
}
/* Main Controller*/
int main(int argc, char *argv[]) {

/* Variables To Include*/
  int sockfd; //socket file descriptor
  int status;
  struct sockaddr_storage player_addr;
  int player1_fd,player2_fd; //file descriptors for both players (essentially newfd)
  int how; //use this to prevent a player from sending after his turn
  struct addrinfo hints, *res;
  struct addrinfo *servinfo;
  socklen_t addr_size;
  int yes = 1;
  int playersNeeded = 2;
  int x = 0; //x coordinate in grid
  int y = 0; //y coordinate in grid
  char buf[8096];
  
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
  printf("Battleship Server : Waiting for 2 More Players to Connect...\n");

  /*Get Players*/
  while(playersNeeded != 1) {
    addr_size = sizeof(player_addr);
    player1_fd = accept(sockfd,(struct sockaddr *)&player_addr, &addr_size);
    if (player1_fd == -1) {
      perror("Error in Connecting Player to Server");
      continue;
    } else {
      --playersNeeded;
      strncpy(buf, "Battleship Server : Waiting for 1 More Player to Connect...\n", 8095);
      buf[8096] = '\0';
      printf("%s", buf);
      write(player1_fd, buf, 60); 
    }
  }

  while(playersNeeded != 0) {
    addr_size = sizeof(player_addr);
    player2_fd = accept(sockfd,(struct sockaddr *)&player_addr, &addr_size);
    if (player2_fd == -1) {
      perror("Error in Connecting Player to Server");
      continue;
    } else {
      --playersNeeded;
      strncpy(buf, "All players connected.\nServer is generating Battlefield grids for both players...\n", 8095);
      buf[8096]= '\0';
      printf("%s", buf);
      write(player1_fd,buf, 82);
      write(player2_fd, buf, 82);
    }
  }
  strcpy(t_por,"+----");
  strcpy(m_por,"|    ");

  //---Generate the grids for both players on server
  generateGrid(x,y);
  //---Ask players to place ships on grid as chosen
  
  return 0;
}
