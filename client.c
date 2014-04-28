#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MYPORT 3369

int main (int argc, char *argv[]) {
	int i;
	int port;
	int sockfd;
	char arg;
	char buf[8096];
	char *address;
	char input[100];
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
	read(sockfd, buf, 61);
        printf("%s", buf);

	//Message letting players know they are connected to an opponent.
	memset(buf, 0, strlen(buf));
	read(sockfd, buf, 83);
        printf("%s", buf);

        //Message containing the player's initial grid.
	memset(buf, 0, strlen(buf));
	read(sockfd, buf, 1337);
        printf("%s", buf);

        //Message containing the key explaining the grid's symbols.
	memset(buf, 0, strlen(buf));
	read(sockfd, buf, 104);
        printf("%s", buf);

	//Notification about each player's ships.
	memset(buf, 0, strlen(buf));
	read(sockfd, buf, 117);
	printf("%s", buf);

	//Messages about placing ships.
        for (i = 0; i < 5; i++) {
		//Choosing orientation.
		memset(buf, 0, strlen(buf));
		read(sockfd, buf, 167);
        	printf("%s", buf);
 	      	scanf("%s", input);
		write(sockfd, input, strlen(input));

		//Choosing first row or column.
		memset(buf, 0, strlen(buf));
		read(sockfd, buf, 81);
		printf("%s", buf);
		memset(input, 0, strlen(input));
		scanf("%s", input);
		write(sockfd, input, strlen(input));

		//Choosing second row or column.
		memset(buf, 0, strlen(buf));
		read(sockfd, buf, 74);
		printf("%s", buf);
		memset(input, 0, strlen(input));
		scanf("%s", input);
		write(sockfd, input, strlen(input));

		//Choosing third row or column.
		memset(buf, 0, strlen(buf));
		read(sockfd, buf, 26);
		printf("%s", buf);
		memset(input, 0, strlen(input));
		scanf("%s", input);
		write(sockfd, input, strlen(input));

		//Message containing the player's grid.
		memset(buf, 0, strlen(buf));
		read(sockfd, buf, 1337);
		printf("%s", buf);
      	}

	//Message about how the server decides who goes first.
	memset(buf, 0, strlen(buf));
	read(sockfd, buf, 85);
	printf("%s", buf);

	//Server letting the players know who is going first.
	memset(buf, 0, strlen(buf));
	read(sockfd, buf, 23);
	printf("%s", buf);

	//Main game loop.
	while (1) {
		//Receive your grid.
                memset(buf, 0, strlen(buf));
                read(sockfd, buf, 1337);
                printf("%s", buf);

		//Enter a row to attack.
		memset(buf, 0, strlen(buf));
		read(sockfd, buf, 26);
		printf("%s", buf);
		memset(input, 0, strlen(input));
		scanf("%s", input);
		write(sockfd, input, strlen(input));

		//Enter a column to attack.
		memset(buf, 0, strlen(buf));
		read(sockfd, buf, 29);
		printf("%s", buf);
		memset(input, 0, strlen(input));
		scanf("%s", input);
		write(sockfd, input, strlen(input));

		//Message about success of attack.
		memset(buf, 0, strlen(buf));
		read(sockfd, buf, 48);
		printf("%s", buf);

		//Gotta figure out how to handle the win condition.
	}

	return 0;
}
