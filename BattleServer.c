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
#include <math.h>

/* Definitions */
#define PORT "3369" //port that is used for server and players
#define MAX_PLAYERS 2 //max num of players
#define DIM 11 //dimension
#define TOTSHIP 5 //total number of ships per player
#define TRUE 1
#define FALSE 0

//int players[MAX_PLAYERS]; //store IP address of player
//int turn = 1; //whose turn is it? (odd for player 1 and even for player 2)
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
char buf[8096]; //buffer
/*typedef struct {
  char* airLoc[5]; //pointer to location of air carrier
  char* bsLoc[4]; //pointer to location of battleship
  char* subLoc[3]; //pointer to location of submarine
  char* cruLoc[3]; //pointer to location of cruiser
  char* patLoc[2]; //pointer to location of patrol boat
  } GridLoc;*/
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
/*GridLoc command1; //ship locations of player 1
  GridLoc command2; //ship location of player 2*/
int life1 = 17; //number of S on grid for player 1
int life2 = 17; //number of S on grid for player 2

void attack(int player_fd, int playerNum) {

  int done = 0;
  int row;
  int column;

  while (done == 0) {
    //Get row.
    strncpy(buf, "Please enter a row, A-J.\n", 8095);
    write(player_fd, buf, strlen(buf));
    memset(buf, 0, strlen(buf));
    read(player_fd, buf, 8095);

    while((int)(buf[0]) < 64 || (int)(buf[0]) > 75) { //based on ASCII table
      strncpy(buf,"Incorrect Row entered. Please try again (A-J).\n", 8095);
      write(player_fd, buf, strlen(buf));
      read(player_fd, buf, 8095);
    }

    row = (int)(buf[0]) - 64;

    //Get column.
    strncpy(buf, "Please enter a column, 0-9.\n", 8095);
    write(player_fd, buf, strlen(buf));
    memset(buf, 0, strlen(buf));
    read(player_fd, buf, 8095);

    while((int)(buf[0]) < 48 || (int)(buf[0]) > 57) { //based on ASCII table
      strncpy(buf,"Incorrect Column entered. Please try again (0-9).\n", 8095);
      write(player_fd, buf, strlen(buf));
      read(player_fd, buf, 8095);
    }

    //Check if area was already attacked.
    if (playerNum == 1) {
      if (pl_2.grid[row][column] == 'H' || pl_2.grid[row][column] == 'M') {
	strncpy(buf, "You've already attacked this location.\n", 8095);
	write(player_fd, buf, strlen(buf));
	//read(player_fd, buf, 8095);
	continue;
      } else if (pl_2.grid[row][column] == 'o') {
	pl_2.grid[row][column] = 'M';
	strncpy(buf, "You have missed the opponent.\n", 8095);
	write(player_fd, buf, strlen(buf));
	//read(player_fd, buf, 8095);
      } else if (pl_2.grid[row][column] == 'S') {
	pl_2.grid[row][column] = 'H';
	strncpy(buf, "You have hit the opponent!\n", 8095);
	write(player_fd, buf, strlen(buf));
	//read(player_fd, buf, 8095);
        life2--; //player2 loses one life point
      }
    } else {
      if (pl_1.grid[row][column] == 'H' || pl_1.grid[row][column] == 'M') {
	strncpy(buf, "You've already attacked this location.\n", 8095);
	write(player_fd, buf, strlen(buf));
        read(player_fd, buf, 8095);
        continue;
      } else if (pl_1.grid[row][column] == 'o') {
        pl_1.grid[row][column] = 'M';
        strncpy(buf, "You have missed the opponent.\n", 8095);
	write(player_fd, buf, strlen(buf));
        read(player_fd, buf, 8095);
      } else if (pl_1.grid[row][column] == 'S') {
	pl_1.grid[row][column] = 'H';
        strncpy(buf, "You have hit the opponent!\n", 8095);
	write(player_fd, buf, strlen(buf));
        read(player_fd, buf, 8095);
        life1--; //player1 loses one life point
      }
    }
  }
}

/*Set location of ship on grid*/
void setLocation(int size,int orientation,int player_fd,int playerNum) {

  int ready = FALSE;
  int row,row1,row2;
  int column,column1,column2 = 0;
  int j;
  int temp;
  int counter;

  while(ready != TRUE) {
    counter = 0;
    if(orientation == 0) { //ship is horizontal
    //Ask for the row and then based on the size of the ship
    //get the col
    strncpy(buf,"\nHorizontal Orientation Selected. Please choose a row from A-J to place ship.",8095);
    write(player_fd, buf, strlen(buf));
    memset(buf, 0, strlen(buf));
    read(player_fd, buf, 8095);
    while((int)(buf[0]) < 64 || (int)(buf[0]) > 75) { //based on ASCII table
      strncpy(buf,"Incorrect Row entered. Please try again (A-J).\n",8095);
      write(player_fd, buf, strlen(buf));
      read(player_fd, buf, 8095);
    }
    row = (int)(buf[0])-64;
    strncpy(buf,"Now, please choose two columns from 0-9 to place ship.\n Enter column number 1.\n",8095);
    write(player_fd,buf,strlen(buf));
    memset(buf, 0, strlen(buf));
    read(player_fd, buf, 8095);
    do {
      while((int)(buf[0]) < 48 || (int)(buf[0]) > 57) {
      strncpy(buf,"Entered column number 1 is invalid. Try again.\n",8095);
      write(player_fd,buf,strlen(buf));
      read(player_fd, buf, 8095);
    }
    column1 = buf[0] - 48;
    strncpy(buf,"Enter column number 2.\n",8095);
    write(player_fd,buf,strlen(buf));
    memset(buf, 0, strlen(buf));
    read(player_fd, buf, 8095);
    while((int)(buf[0]) < 48 ||(int)(buf[0]) >57) {
      strncpy(buf,"Entered column number 2 is invalid. Try again.\n",8095);
      write(player_fd,buf,strlen(buf));
      read(player_fd, buf, 8095);
    }
    column2 = buf[0] - 48;
    } while(abs(column1-column2) != size-1);
    //check to finally make sure spaces are 'o'
    if(playerNum == 1) {
      if(column2 < column1) {
        temp = column1;
        column1 = column2;
        column2 = temp;
      }
      for(j = column1; j <= column2; j++) {
        if(pl_1.grid[row][j] != 'o') {
	  break;
	}
	counter++;
      }
      if(counter == size) {
       	for(j = column1; j <= column2; j++) {
	  pl_2.grid[row][j] = 'S';
	}
        ready = TRUE;
      }
    } else {
      if(column2 < column1) {
        temp = column1;
        column1 = column2;
        column2 = temp;
      }
      for(j = column1; j <= column2; j++) {
        if(pl_2.grid[row][j] != 'o') {
          break;
        }
        counter++;
      }
      if(counter == size) {
        for(j = column1; j <= column2; j++) {
	  pl_2.grid[row][j] = 'S';
	}
        ready = TRUE;
      }
    }
    } else { //ship is vertical (orient = 1)
      strncpy(buf,"Vertical Orientation Selected. Please choose a column from 0-9 to place ship.",8095);
      write(player_fd, buf, strlen(buf));
      memset(buf, 0, strlen(buf));
      read(player_fd, buf, 8095);
      while((int)(buf[0]) < 48 || (int)(buf[0]) > 57) { //based on ASCII table
	strncpy(buf,"Incorrect Column entered. Please try again (0-9).\n",8095);
	write(player_fd, buf, strlen(buf));
	read(player_fd, buf, 8095);
      }
      column = (int)(buf[0])-47;
      strncpy(buf,"Now, please choose two rows from A-J to place ship.\nEnter row 1.\n",8095);
      write(player_fd,buf,strlen(buf));
      memset(buf, 0, strlen(buf));
      read(player_fd, buf, 8095);
      do {
	while((int)(buf[0]) < 64 || (int)(buf[0]) > 75) {
	  strncpy(buf,"Entered row number 1 is invalid. Try again.\n",8095);
	  write(player_fd,buf,strlen(buf));
	  read(player_fd, buf, 8095);
	}
	row1 = buf[0] - 64;
	strncpy(buf,"Enter row number 2.\n",8095);
	write(player_fd,buf,strlen(buf));
	memset(buf, 0, strlen(buf));
	read(player_fd, buf, 8095);
	while((int)(buf[0]) < 64 ||(int)(buf[0]) > 75) {
	  strncpy(buf,"Entered row number 2 is invalid. Try again.\n",8095);
	  write(player_fd,buf,strlen(buf));
	  read(player_fd, buf, 8095);
	}
	row2 = buf[0] - 64;
      } while(abs(row1-row2) != size-1);
      if(playerNum == 1) {
	if(row2 < row1) {
	  temp = row1;
	  row1 = row2;
	  row2 = temp;
	}
	for(j = row1; j <= row2; j++) {
	  if(pl_1.grid[j][column] != 'o') {
	    break;
	  }
	  counter++;
	}
	if(counter == size) {
          for(j = row1; j <= row2; j++) {
	    pl_1.grid[j][column] = 'S';
          }
	  ready = TRUE;
	}
      } else {
	if(row2 < row1) {
	  temp = row1;
	  row1 = row2;
	  row2 = temp;
	}
	for(j = row1; j <= row2; j++) {
	  if(pl_2.grid[j][column] != 'o') {
	    break;
	  }
	  counter++;
	}
	if(counter == size) {
          for(j = row1; j <= row2; j++) {
            pl_2.grid[j][column] = 'S';
	  }
          ready = TRUE;
	}
      }
    }
  }
}
/*Position horizontally or vertically*/
void positionShip(char* type,int size,int player_fd,int playerNum) {

  int orientation = 2;//vert or horiz
  char temp[5];
  /*Let server know what ship player must place*/
  printf("Player %d is placing %s of size %d...\n",playerNum,type,size);
  //Tell player to choose horizontal or vertical position of ship
  while (orientation == 2) {
    memset(buf, 0, strlen(buf));
    strncpy(buf, "\nThe current ship to set is a(n) ", 8095);
    strncat(buf, type, 8095);
    strncat(buf, ", and is ", 8095);
    sprintf(temp, "%d", size);
    strncat(buf, temp, 1);
    strncat(buf, " units long.\nDo you want to set this ship horizontally or vertically? H for horizontal, V for vertical.\n", 8095); 
    write(player_fd, buf, strlen(buf));
    memset(buf, 0, strlen(buf));
    read(player_fd, buf, 8095);
    if (buf[0] == 'H' || buf[0] == 'h') {
      orientation = 0;
    } else if (buf[0] == 'V' || buf[0] == 'v') {
      orientation = 1;
    }
  }
  setLocation(size,orientation,player_fd,playerNum);
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
void printKey(int player1_fd, int player2_fd) {
  strncpy(buf, "\nBattleship Key :\n", 8095);
  strncat(buf, "S stands for (S)hip\n", 8095);
  strncat(buf, "o stands for (o)pen Waters\n", 8095);
  strncat(buf, "H stands for (H)it\n", 8095);
  strncat(buf, "M stands for (M)iss aaaaaaaaaa\n", 8095);

  write(player1_fd, buf, strlen(buf));
  write(player2_fd, buf, strlen(buf));
}

/*Print out Grid*/
void printGrid1(int player1_fd) {

  int i,j = 0;

  strncpy(buf, "\nPlayer 1 Battleship Grid\n", 8095);
  for(j = 0; j < grid_height; ++j) {
    for(i = 0; i < grid_width; ++i) {
      strncat(buf, t_por, 8095);
      if(i == (grid_width-1)) {
	strncat(buf, "+", 8095);
      }
    }
    strncat(buf, "\n", 8095);
    for(i = 0; i < grid_width;++i) {
      strncat(buf, m_por, 8095);
      strncat(buf, &pl_1.grid[j][i], 1);
      if(i == (grid_width-1)) {
	strncat(buf, "|", 8095);
      }
    }
    strncat(buf, "\n", 8095);
    if (j == (grid_height-1)) {
      for (i=0; i<grid_width; ++i) {
	strncat(buf, t_por, 8095);
	if (i == (grid_width - 1)) {
	  strncat(buf, "+", 8095);
	}
      }
    }
  }
  strncat(buf, "\n", 8095); printf("%s", buf);
  write(player1_fd, buf, strlen(buf));
}

void printGrid2(int player2_fd) {

  int i,j = 0;

  strncpy(buf,"\nPlayer 2 Battleship Grid\n",8095);
  for(j = 0; j < grid_height; ++j) {
    for(i = 0; i < grid_width; ++i) {
      strncat(buf, t_por, 8095);
      if(i == (grid_width-1)) {
        strncat(buf, "+", 8095);
      }
    }
    strncat(buf, "\n", 8095);
    for(i = 0; i < grid_width;++i) {
      strncat(buf, m_por, 8095);
      strncat(buf, &pl_2.grid[j][i], 1);
      if(i == (grid_width-1)) {
        strncat(buf, "|", 8095);
      }
    }
    strncat(buf, "\n", 8095);
    if (j == (grid_height-1)) {
      for (i=0; i<grid_width; ++i) {
        strncat(buf, t_por, 8095);
        if (i == (grid_width - 1)) {
          strncat(buf, "+", 8095);
        }
      }
    }
  }
  strncat(buf, "\n", 8095);
  write(player2_fd, buf, strlen(buf));
}

/* Update Grid*/
void updateGrid(int playerNum,int player1_fd,int player2_fd) {

  if((playerNum) == 1) { //player 1 turn
    printGrid1(player1_fd);
  } else { //player 2 turn
    printGrid2(player2_fd);
  }
}

/* Grid Generator*/
void generateGrid(int x, int y, int player1_fd, int player2_fd) {

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
  printGrid1(player1_fd);
  printGrid2(player2_fd);
}

void outcome1(int player1_fd,int player2_fd) {
  strncpy(buf,"\nYou win!!!\n",8095);
  buf[8095] = '\0';
  write(player1_fd,buf,strlen(buf));
  strncpy(buf,"\nYou lose...\n",8095);
  buf[8095] = '\0';
  write(player2_fd,buf,strlen(buf));
}

void outcome2(int player1_fd, int player2_fd) {
  strncpy(buf,"\nYou win!!!\n",8095);
  buf[8095] = '\0';
  write(player2_fd,buf,strlen(buf));
  strncpy(buf,"\nYou lose...\n",8095);
  buf[8095] = '\0';
  write(player1_fd,buf,strlen(buf));
}

/*Battle Method where Players Battle*/
void battle(int playerNum, int player1_fd, int player2_fd) {
  while(life1 != 0 || life2 != 0) {
    if(life1 == 0) {
      outcome2(player1_fd,player2_fd); //player1 loses and player2 wins
      break;
    }
    if(life2 == 0) {
      outcome1(player1_fd,player2_fd); //player2 loses and player1 wins
      break;
    }
    if(playerNum == 1) {
      attack(player1_fd,playerNum);
      updateGrid(playerNum,player1_fd,player2_fd);
      playerNum = 2;
    } else {
      attack(player2_fd,playerNum);
      updateGrid(playerNum,player1_fd,player2_fd);
      playerNum = 1;
    }
  }
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
      memset(buf, 0, strlen(buf));
      strncpy(buf, "\nNow connected to the server.\n", 8095);
      buf[8095]= '\0';
      write(player2_fd, buf, strlen(buf));

      memset(buf, 0, strlen(buf));
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
  memset(buf, 0, strlen(buf));
  generateGrid(x,y,player1_fd,player2_fd);
  memset(buf, 0, strlen(buf));
  printKey(player1_fd, player2_fd);
  //---Ask players to place ships on grid as chosen
  initializeArmies(); //initialize armies of two players
  printf("\n");
  memset(buf, 0, strlen(buf));
  strncpy(buf,"\nShip sizes and types of both armies are initalized on Server.\nPlease wait for a notification to position your army.\n",8095);
  buf[8095] = '\0';
  printf("%s", buf);
  write(player1_fd, buf, strlen(buf));
  write(player2_fd, buf, strlen(buf));

  //---Tell player 2 to wait while player 1 places his ships
 /* strncpy(buf,"\nPlease wait while player 1 places his ships.\n",8095);
  buf[8095] = '\0';
  write(player2_fd, buf, strlen(buf));*/
  how = 1; //further sends are disallowed
  //---Ask player 1 to position each of his ships
  while(numShips1 != 0) {
    for(ship = 0; ship < TOTSHIP; ship++) {
      //start with air carrier and end with patrol boat
      positionShip(army1[ship].type, army1[ship].size, player1_fd, playerNum);
      updateGrid(playerNum,player1_fd,player2_fd);
      numShips1--;
    }
  }
  playerNum = 2; //turn is even so its player 2 turn
  /*strncpy(buf,"\nPlease wait while player 2 places his ships.\n",8095);
  buf[8095] = '\0';
  write(player2_fd, buf, strlen(buf));*/
  playerNum = 2;
  //---Ask player 2 to position each of his ships
  while(numShips2 != 0) {
    for(ship = 0;ship < TOTSHIP;ship++) {
      //start with air carrier and end with patrol boat
      positionShip(army2[ship].type, army2[ship].size, player2_fd, playerNum);
      updateGrid(playerNum,player1_fd,player2_fd);
      numShips1--;
    }
  }
  strncpy(buf,"\nPlease wait as the Server decides which player goes first. This is a random process\n",8095);
  write(player1_fd, buf, strlen(buf));
  write(player2_fd, buf, strlen(buf));
  playerNum = rand() %2 + 1; //generate either 1 or 2
  if(playerNum == 1) {
    strncpy(buf,"Player 1 will go first\n",8095);
    buf[8095] = '\0';
    write(player1_fd, buf, strlen(buf));
    write(player2_fd, buf, strlen(buf));
    battle(playerNum,player1_fd,player2_fd);
  } else {
    strncpy(buf,"Player 2 will go first\n",8095);
    buf[8095] ='\0';
    write(player1_fd, buf, strlen(buf));
    write(player2_fd, buf, strlen(buf));
    battle(playerNum,player1_fd,player2_fd);
  }
  close(player1_fd);
  close(player2_fd);
  close(sockfd);
  return 0;
}
