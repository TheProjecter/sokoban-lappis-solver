/**
 * KTH - Royal Institute of Technology
 * DD2380 - Artificial Intelligence
 * Autumn 2010
 *
 * Client used for conecting to the server
 * and getting boards.
 *
 * @author  Kevin Anceau <anceau@kth.se>
 * @author  Andrea Baisero <baisero@kth.se>
 * @author  Carlos Alberto Colmenares <cacol@kth.se>
 * @author  Manuel Parras Ruiz De Azea <mprda@kth.se>
 *
 */
#include"client.h"

void error(const char* format, ... ) {
    va_list args;
    fprintf( stderr, "Error: " );
    va_start( args, format );
    vfprintf( stderr, format, args );
    va_end( args );
    fprintf( stderr, "\n" );
    exit(1);
}

//In case there's no internet connection
//just read a selected file
void read_from_file(char *filename,char **sol){
    FILE *file;
    char buffer[BUFFERSIZE];
    int n;

    file = fopen( filename , "r");
    if(file!=NULL){
        n = fread(buffer, 1, BUFFERSIZE, file);
        fclose(file);

        printf("BOARD:\n%s\n",buffer);
        *sol = solve_sokoban(buffer, n);
    }
    else
        error("opening file '%s'",filename);
}

void read_from_server(char *board,char **sol){
    const char *ADDRESS ="cvap103.nada.kth.se";
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[BUFFERSIZE];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("opening socket");
    server = gethostbyname(ADDRESS);
    if (server == NULL)
        error("no such host");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(PORT);
    if (connect(sockfd,(const struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0) 
        error("connecting");
    n = write(sockfd,board,strlen(board));
    if (n < 0) 
         error("writing to socket");
    bzero(buffer,BUFFERSIZE);
    n = read(sockfd,buffer,BUFFERSIZE-1);
    if (n < 0) 
         error("reading from socket");
    
    printf("BOARD:\n%s\n",buffer);
    *sol = solve_sokoban(buffer, n);

    n = write(sockfd,*sol,strlen(*sol));
    if (n < 0) 
         error("writing to socket");

    bzero(buffer,BUFFERSIZE);

    n = read(sockfd,buffer,BUFFERSIZE-1);
    if (n < 0) 
        error("reading from socket");

    printf("%s\n",buffer);
}

int main(int argc, char *argv[])
{
    // parameter check
    if( (argc!=2 && argc!=3) || (argc==3 && strcmp(argv[1],"-f")!=0) ){
        error("usage %s [-f] boardnum\n",argv[0]);
    }

    char *sol;
    if( argc==2 ){ /* 1 parameter means that board should be read from server */
        read_from_server(argv[1],&sol);
    }
    else{ /* otherwise board should be read from file */
        read_from_file(argv[2],&sol);
    }

    cout << "SOLUTION:" << endl << sol << endl << endl;

    exit(0);
}
