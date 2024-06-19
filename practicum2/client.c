#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 4) {
       fprintf(stderr,"usage %s hostname port command file_name\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    // Create the request string with the command and file name
    char request[256];
    sprintf(request, "%s %s", argv[3], argv[4]);

    // Send the request to the server
    n = write(sockfd,request,strlen(request));
    if (n < 0) 
         error("ERROR writing to socket");

    // Clear the buffer
    bzero(buffer,256);

    // Read the response from the server
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");

    // Print the response from the server
    printf("%s\n",buffer);

    // Close the connection
    close(sockfd);
    return 0;
}
