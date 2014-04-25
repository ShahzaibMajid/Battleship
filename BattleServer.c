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
#define TOTSHIP 5 //total number of ships per player
#define TRUE 1
#define FALSE 0

int players[MAX_PLAYERS]; //store IP address of player
int turn = 1; //whose turn is it? (odd for player 1 and even for player 2)
/* Grid Stuff*/
typedef struct {
  char grid[DIM][DIM]; //11 x 11 grid (actually 10 x 10)
} Grid;
Grid pl_1; //grid of player 1
Grid pl_2; //grid of player 2
int grid_height = 11;
int grid_width = 11;
char t_por[7]; //top portion of the box
char m_por[7]; //middle portion of the box
typedef struct {
  char* airLoc[5]; //pointer to location of air carrier
  char* bsLoc[4]; //pointer to location of battleship
  char* subLoc[3]; //pointer to location of submarine
  char* cruLoc[3]; //pointer to location of cruiser
  char* patLoc[2]; //pointer to location of patrol boat
} GridLoc;
/* Grid Stuff Completed*/

/* Ship Stuff*/
typedef struct {
  int size; //size of the ship
  int hits; //hit points or health points
  char* position; //horizontal or vertical position
  char* type; //type of ship
  /* Types of ship        sizes
     aircraft carrier     5
     battleship           4
     submarine            3
     cruiser              3
     patrol boat          2
   */
} Ship;
int numShips1 = 5; //number of ships for player 1
int numShips2 = 5; //number of ships for player 2
Ship army1[5]; //army #1 for player 1
Ship army2[5]; //army #2 for player 2
GridLoc command1; //ship locations of player 1
GridLoc command2; //ship location of player 2
/*Set location of ship on grid*/
void setLocation(int size) {

}

/*Position horizontally or vertically*/
void positionShip(char* type,int size,int player_fd,int playerNum) {

  int ready = FALSE;
  /*Let server know what ship player must place*/
  printf("Player %d is placing %s of size %d...\n",playerNum,type,size);
  //Tell player to choose horizontal or vertical position of ship
  while(ready != TRUE) {


  }
}

void initializeArmies() {

/*Initialize Army #1*/
  army1[0].type = "aircraft carrier";
  army1[0].size = 5;
  army1[1].type= "battleship";
  army1[1].size = 4;
  army1[2].type= "submarine";
  army1[2].size = 3;
  army1[3].type= "cruiser";
  army1[3].size = 3;
  army1[4].type= "patrol boat";
  army1[4].size = 2;

/*Initialize Army #2*/
  army2[0].type = "aircraft carrier";
  army2[0].size = 5;
  army2[1].type= "battleship";
  army2[1].size = 4;
  army2[2].type= "submarine";
  army2[2].size = 3;
  army2[3].type= "cruiser";
  army2[3].size = 3;
  army2[4].type= "patrol boat";
  army2[4].size = 2;
}

/*Print out Key of Battleship*/
void printKey() {
  printf("\nBattleship Key :\n");
  printf("S stands for (S)hip\n");
  printf("o stands for (o)pen Waters\n");
  printf("H stands for (H)it\n");
  printf("M stands for (M)iss");
}

/*Print out Grid*/
void printGrid1() {

  int i,j = 0;

  printf("\nPlayer 1 Battleship Grid\n");
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
      printf("%c", pl_1.grid[j][i]);
      if(i == (grid_width-1)) {
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
  printKey();
}

void printGrid2() {

  int i,j = 0;

  printf("\nPlayer 2 Battleship Grid\n");
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
      printf("%c", pl_1.grid[j][i]);
      if(i == (grid_width-1)) {
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
  printKey();
}

/* Update Grid*/
void updateGrid(int turn) {

  if((turn%2) == 1) { //player 1 turn
    printGrid1();
  } else { //player 2 turn
    printGrid2();
  }
}

/* Grid Generator*/
void generateGrid(int x, int y) {

  int ascii = 65; //ascii value of 'A'
  int pos = 48; //first position on grid for row

  x = 0;
  pl_1.grid[0][0] = '#'; //marks no man land coordinate in grid
  pl_2.grid[0][0] = '#';

  //Set numbers on columns of the grid (#1-10)
  for(y = 1;y < DIM; y++) {
    pl_1.grid[x][y] = (char)pos;
    pl_2.grid[x][y] = (char)(pos++);
  }

  y = 0;
  //Set Letters A-J on rows of the grid
  for(x = 1; x < DIM; x++) {
    pl_1.grid[x][y] =  (char)ascii;
    pl_2.grid[x][y] =  (char)(ascii++);
  }

  //Set battlefield with (o)pen Water character
  for(x = 1; x < DIM; x++) {
    for(y = 1; y < DIM; y++) {
      pl_1.grid[x][y] = 'o'; //o stands for (o)pen water
      pl_2.grid[x][y] = 'o';
    }
  }
  printGrid1();
  printGrid2();
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
  int ship; //specific ship in army
  int playerNum = 1; //are you player x?

  printf("\n");

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
    player1_fd = accept(sockfd, (struct sockaddr *)&player_addr, &addr_size);
    if (player1_fd == -1) {
      perror("Error in Connecting Player to Server");
      continue;
    } else {
      --playersNeeded;
      strncpy(buf, "\nBattleship Server : Waiting for 1 More Player to Connect...\n", 8095);
      buf[8095] = '\0';
      printf("%s", buf);
      write(player1_fd, buf, strlen(buf));
    }
  }

  while(playersNeeded != 0) {
    addr_size = sizeof(player_addr);
    player2_fd = accept(sockfd, (struct sockaddr *)&player_addr, &addr_size);
    if (player2_fd == -1) {
      perror("Error in Connecting Player to Server");
      continue;
    } else {
      --playersNeeded;
      strncpy(buf, "\nAll players connected.\nServer is generating Battlefield grids for both players...\n", 8095);
      buf[8095]= '\0';
      printf("%s", buf);
      write(player1_fd, buf, strlen(buf));
      write(player2_fd, buf, strlen(buf));
    }
  }
  strcpy(t_por,"+----");
  strcpy(m_por,"|   ");

  //---Generate the grids for both players on server
  generateGrid(x,y);
  //---Ask players to place ships on grid as chosen
  initializeArmies(); //initialize armies of two players
  printf("\n");
  strncpy(buf,"\nShip sizes and types of both armies are initalized on Server.\nPlease position your army.\n",8095);
  buf[8095] = '\0';
  printf("%s", buf);
  write(player1_fd, buf, strlen(buf));
  write(player2_fd, buf, strlen(buf));

  //---Tell player 2 to wait while player 1 places his ships
  strncpy(buf,"\nPlease wait while player 1 places his ships.\n",8095);
  buf[8095] = '\0';
  write(player2_fd, buf, strlen(buf));
  how = 1; //further sends are disallowed
  //---Ask player 1 to position each of his ships
  while(numShips1 != 0) {
    for(ship = 0; ship < TOTSHIP; ship++) {
      //start with air carrier and end with patrol boat
      positionShip(army1[ship].type, army1[ship].size, player1_fd, playerNum);
      updateGrid(turn);
      numShips1--;
    }
  }
  ++turn; //turn is even so its player 2 turn
  if(shutdown(player1_fd, how) == -1) { //player 1 must now wait and cannot send to server
    perror("Error in prohibiting player 2 from sending when it is not his turn.");
  }
  strncpy(buf,"\nPlease wait while player 2 places his ships.\n",8095);
  buf[8095] = '\0';
  write(player2_fd, buf, strlen(buf));
  playerNum = 2;
  //---Ask player 2 to position each of his ships
  while(numShips2 != 0) {
    for(ship = 0;ship < TOTSHIP;ship++) {
      //start with air carrier and end with patrol boat
      positionShip(army2[ship].type, army2[ship].size, player2_fd, playerNum);
      updateGrid(turn);
      numShips1--;
    }
  }
  return 0;
}
