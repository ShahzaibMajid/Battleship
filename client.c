#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//hey 
#define MYPORT 3369

int main (int argc, char *argv[]) {
	int i;
	int port;
	int sockfd;
	char arg;
	char buf[8096];
	char *address;
	static struct sockaddr_in serv_addr;
	struct hostent *he;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}


	port = MYPORT;
	address = (char *)"localhost";

	while ((arg = getopt(argc, argv, "p:")) != -1) {
		switch (arg) {
			case 'p':
				port = atoi(optarg);
				break;
			case '?':
				printf("\n\nNot a correct option.\n\n");
				break;
		}
	}

	if ((he = gethostbyname(address)) == NULL) {
		perror("gethostbyname");
		exit(1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr = *((struct in_addr *)he->h_addr);
	serv_addr.sin_port = htons(port);

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))
	== -1) {
		perror("connect");
		exit(1);
	}

	//Initial message when connecting to server.
	read(sockfd, buf, 8096);
        printf("%s", buf);

	//Message letting players know they are connected to an opponent.
	read(sockfd, buf, 8096);
        printf("%s", buf);
        
        //Message containing the player's initial grid.
	read(sockfd, buf, 8096);
        printf("%s", buf);
        
        //Message containing the key explaining the grid's symbols.
	read(sockfd, buf, 8096);
        printf("%s", buf);

	//Messages about placing ships.
        for (i = 0; i < 5; i++) {
	read(sockfd, buf, 8096);
        printf("%s", buf);
        scanf("%c", &input);
        printf("\n%c\n", input);
        }


	return 0;
	return 0;
}
