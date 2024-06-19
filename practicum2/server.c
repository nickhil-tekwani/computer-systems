#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <time.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    // Check for the correct number of command line arguments
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    // Clear the address structure
    bzero((char *) &serv_addr, sizeof(serv_addr));

    // Get the port number
    portno = atoi(argv[1]);

    // Set up the address structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind the socket to the specified port
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

    // Listen for incoming connections
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    // Accept an incoming connection
    newsockfd = accept(sockfd, 
                (struct sockaddr *) &cli_addr, 
                &clilen);
    if (newsockfd < 0) 
         error("ERROR on accept");

    // Clear the buffer
    bzero(buffer,256);

    // Read data from the client
    n = read(newsockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
	//	// find the first instance of the delimiter
//	char *delimiter = strchr(buffer, '|');
//
//	// variables
//	char command[256];
//	char file_name[256];
//	
//	// check if the delimiter was found
//	if (delimiter == NULL) {
//		printf("Invalid command or file name\n");
//	} else {
//		// copy the part of the string before the delimiter into a new string
//		strncpy(command, buffer, delimiter - buffer);
//		command[delimiter - buffer] = '\0'; // add a null terminator
//
//		// copy the part of the string after the delimiter into a new string
//		
//		strcpy(file_name, delimiter + 1);
//
//		// print the parsed values
//		printf("Command: %s\n", command);
//		printf("File Path: %s\n", file_name);
//	}
	
	// Parse the command and file path from the received message
    char *copy = strdup(buffer);
	char *command;
	if (copy == NULL) {
    	printf("Error copying string\n");
	} else {
    	command = strtok(copy, " ");
    	free(copy);
	}
    char* file_name = strtok(NULL, " ");
	
	// check for errors
	if (command == NULL) {
    	printf("Invalid command");
	} else if(file_name == NULL) {
		printf("Invalid file name\n");
	} else {
		printf("Command: %s\n", command);
		printf("File Path: %s\n", file_name);
	}
	
	// Print received command and file path
	printf("Command: %s\n", command);
	printf("File Path: %s\n", file_name);
	
    // Print the received message
    // printf("Here is the message: %s\n",buffer);
	
	// QUESTION 2/3
	// Check if the command is "GET"
    if (strcmp(command, "GET") == 0) {
        // Open the specified file
        FILE *file = fopen(file_name, "r");
		if (file == NULL) {
			printf("Error opening file\n");
		} else {
			printf("Successfully opened file\n");
		}
		
        if (file == NULL) {
            // File not found, send an error
			error("ERROR opening file");
		}
		
		// read file into a buffer
		char buffer[1024];
		int bytes_read = fread(buffer, 1, sizeof(buffer), file);
		
		// send file contents back to client
		int bytes_written = write(newsockfd, buffer, bytes_read);
		if (bytes_written < 0) error("ERROR writing to socket");
	}
	
	// QUESTION 3
	// use the stat function to get information about the file
	struct stat file_info;
	if (stat(file_name, &file_info) < 0) {
		perror("Error getting file information");
	}
	// print the file size
	printf("Size: %lld bytes\n", file_info.st_size);
	// print the file permissions
	printf("Permissions: %o\n", file_info.st_mode);
	// print the time of last modification
	char last_modified[256];
	strftime(last_modified, sizeof(last_modified), "%c", localtime(&file_info.st_mtime));
	printf("Last modified: %s\n", last_modified);
	
	// QUESTION 4
	// Check if the command is "MKDIR"
	if (strcmp(command, "MKDIR") == 0) {
		// Create the specified directory
		int result = mkdir(file_name, 0777);
		char* status_message;
		if (result == 0) {
			// Directory created successfully
			status_message = "Directory created successfully\n";
			write(newsockfd, status_message, strlen(status_message));
		} else {
			// Error creating directory
			status_message = "Error creating directory\n";
			write(newsockfd, status_message, strlen(status_message));
		}
		printf(status_message, "%s");
	}
	
	// QUESTION 5
	// check if the command is "PUT"
	if (strcmp(command, "PUT") == 0) {
		// open the file for writing
		FILE *file = fopen(file_name, "w");

		if (file == NULL) {
			// unable to create the file
			// send an error message back to the client
			send(newsockfd, "ERROR: unable to create file\n", strlen("ERROR: unable to create file\n"), 0);
		} else {
			// file was successfully created
			// send a success message back to the client
			send(newsockfd, "SUCCESS: file created\n", strlen("SUCCESS: file created\n"), 0);

			// read the stream of bytes from the client
			int bytes_read = read(newsockfd, buffer, sizeof(buffer));

			// write the bytes to the file
			fwrite(buffer, 1, bytes_read, file);

			// close the file
			fclose(file);
		}
	}
	
	// QUESTION 6
	// Check if the command is "RM"
	if (strcmp(command, "RM") == 0) {
		// Remove the specified file
		int result = unlink(file_name);
		char* status_message;
		if (result == 0) {
			// File removed successfully
			status_message = "File removed successfully\n";
			write(newsockfd, status_message, strlen(status_message));
		} else {
			// Error removing file
			status_message = "Error removing file\n";
			write(newsockfd, status_message, strlen(status_message));
		}
		printf(status_message, "%s");
	}

		
    // Close the connection
    close(newsockfd);
    close(sockfd);
    return 0;
}
