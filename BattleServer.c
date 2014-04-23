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

/* Definitions */
#define PORT "3330" //port that is used for server and players
#define MAX_PLAYERS 2 //max num of players

int players[MAX_PLAYERS];

/*Get Players*/

int main(void) {

/* Variables To Include*/
  int sockfd; //socket file descriptor
  int status;
  int player1, player2; //file descriptors for both players
  int how; //use this to prevent a player from sending after his turn 
  struct addrinfo hints, *res;
  struct addrinfo *servinfo;

/*Establish Server*/
  memset(&hints,0,sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM; //TCP socket type
  hints.ai_flags = AI_PASSIVE; //my IP is used

  if((status = getaddrinfo(NULL,PORT,&hints,&servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return(1); //Good Job You Failed :D
  }
  


}
