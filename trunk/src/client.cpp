#include"client.h"

// Auxiliary function only for make proofs. After that we can remove it.
void function2 (char buffer[], int size, char **sol){
	
	cout << "The board received is:" << endl << buffer << endl << "size: " << size << endl;
	
	//sleep(50);
	
	char solution[] = "U R R D U U L D L L U L L D R R R R L D D R U R U D L L U R";
	
	*sol = solution;	
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

//In case there's no internet connection
//just read a random file
void read_a_file(char **sol){
    FILE *file;
    char buffer[BUFFERSIZE];
    int n;

    file = fopen("../test-cases/1.skb", "r");

    if(file)
        n = fread(buffer, 1, BUFFERSIZE, file);
    else
        error("ERROR opening file");

    *sol = solve_sokoban(buffer, n);

}

int main(int argc, char *argv[])
{
    if(true){
        char* sol;
        read_a_file(&sol);
        //cout << "Solution found: " << sol << endl;
        return 0;
    }

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
	
	// Now we compute the solution
	
	char *MYSOL;
	
	function2(buffer,n,&MYSOL);
	
    n = write(sockfd,MYSOL,strlen(MYSOL));
    if (n < 0) 
         error("ERROR writing to socket");

    bzero(buffer,BUFFERSIZE);

    n = read(sockfd,buffer,BUFFERSIZE-1);
    if (n < 0) 
         error("ERROR reading from socket");

	printf("%s\n",buffer);
    return 0;
}
