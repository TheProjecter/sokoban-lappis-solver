#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <iostream>
#include <cstdlib>

#define PORT 5555
#define BUFFERSIZE 1024


using namespace std;

// Auxiliary function only for make proofs. After that we can remove it.
void function2 (char buffer[], char **sol){
	
	cout << "The board received is:" << endl << buffer << endl;
	
	sleep(50);
	
	char solution[] = "U R R D U U L D L L U L L D R R R R L D D R U R U D L L U R";
	
	*sol = solution;	
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
	const char *ADDRESS ="cvap103.nada.kth.se";
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[BUFFERSIZE];
    if (argc < 2) {
       fprintf(stderr,"usage %s boardnum\n", argv[0]);
       exit(0);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(ADDRESS);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(PORT);
    if (connect(sockfd,(const struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    n = write(sockfd,argv[1],strlen(argv[1]));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,BUFFERSIZE);
    n = read(sockfd,buffer,BUFFERSIZE-1);
    if (n < 0) 
         error("ERROR reading from socket");
	printf("%s\n",buffer);
	
	// Below this comment goes the call to your function
	
	char *MYSOL;
	
	function2(buffer,&MYSOL);
	
	// In MYSOL we put the solution to send it
	// We only have 60 seconds to find the solution
	// We cannot break the connection because we have to send the solution in the same socket
	
	//char MYSOL[]="U R R D U U L D L L U L L D R R R R L D D R U R U D L L U R";
	//char *MYSOL="0 3 3 1 0 0 2 1 2 2 0 2 2 1 3 3 3 3 2 1 1 3 0 3 0 1 2 2 0 3";
	
    n = write(sockfd,MYSOL,strlen(MYSOL));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,BUFFERSIZE);
    n = read(sockfd,buffer,BUFFERSIZE-1);
	printf("%s\n",buffer);
    return 0;
}
