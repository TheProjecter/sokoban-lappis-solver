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
    exit(1);
}

//In case there's no internet connection
//just read a random file
void read_from_file(char *filename,char **sol){
    FILE *file;
    char buffer[BUFFERSIZE];
    char filepath[]="../test-cases/";
    int n;

    file = fopen( strcat(filepath,filename) , "r");
    if(file!=NULL){
        n = fread(buffer, 1, BUFFERSIZE, file);
        fclose(file);

        printf("BOARD:\n%s\n",buffer);
        *sol = solve_sokoban(buffer, n);
    }
    else{
        fprintf(stderr,"ERROR opening file '%s'",filename);
        exit(1);
    }
}

void read_from_server(char *board,char **sol){
    const char *ADDRESS ="cvap103.nada.kth.se";
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[BUFFERSIZE];
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
    n = write(sockfd,board,strlen(board));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,BUFFERSIZE);
    n = read(sockfd,buffer,BUFFERSIZE-1);
    if (n < 0) 
         error("ERROR reading from socket");
    
    printf("BOARD:\n%s\n",buffer);
    *sol = solve_sokoban(buffer, n);

    n = write(sockfd,*sol,strlen(*sol));
    if (n < 0) 
         error("ERROR writing to socket");

    bzero(buffer,BUFFERSIZE);

    n = read(sockfd,buffer,BUFFERSIZE-1);
    if (n < 0) 
        error("ERROR reading from socket");

    printf("%s\n",buffer);
}

int main(int argc, char *argv[])
{
    // parameter check
    if( (argc!=2 && argc!=3) || (argc==3 && strcmp(argv[1],"-f")!=0) ){
        fprintf(stderr,"usage %s [-f] boardnum\n",argv[0]);
        exit(1);
    }

    char *sol;
    if( argc==2 ){ /* 1 parameter means that board should be read from server */
        read_from_server(argv[1],&sol);
    }
    else{ /* otherwise board should be read from file */
        read_from_file(argv[2],&sol);
    }

    printf("SOLUTION:\n%s\n",sol);

    exit(0);
}
