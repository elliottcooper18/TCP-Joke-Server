#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>

#define kBufSize 512

typedef struct USER{
    char *name;
    int age;
}USER;

void handle_connection(int);
int readLineFromNetwork(int sd, char *buf, int size);
void write_to_client(int sd, char *buf);
void get_user(USER*, int);
int readLineFromNetwork(int sd, char *buf, int size);


int main(int argc, char **argv){

    // how to implment a server - create a socket, bind it to port and ip, listen, accept, close connection, keep going
    // will just implement serial connections

    struct sockaddr_in server_addr, client_addr;
    int server_socket, server_port, client_socket;
    unsigned int client_length;

    // get the port number from command line args
    if (!(argc == 2)){
        fprintf(stderr, "{ERROR} No port number specified\n");
        exit(1);
    }

    server_port = atoi(argv[1]); // get the server port from the command line argument

    // Check for range of the port
    if (!(server_port >= 1024 && server_port <= 65536)){
        fprintf(stderr, "{ERROR} Server port out of range\n");
        exit(1);
    }

    // Create the socket type internet, stream of data, and tcp protocol
    if ((server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
        
        fprintf(stderr, "{ERROR} Cannot create the socket\n");
        exit(1);
    } 

    // Create the server address
    memset(&server_addr, 0, sizeof(server_addr)); // set all data to 0, as some will not be used
    server_addr.sin_family = AF_INET; // set type of connection to be internet
    server_addr.sin_addr.s_addr = INADDR_ANY; // set any ip address to be listening to connection   
    server_addr.sin_port = htons(server_port); // convert to big endian number which is network standard

    // Bind the socket to the port and the ip address
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){

        fprintf(stderr, "{ERROR} Socket could not be binded\n");
        exit(1);
    }

    // Listen to incomming connections on server socket
    listen(server_socket, 10);

    // Accept connection and handle
    while(1){
        
        client_length = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_length);

        handle_connection(client_socket);

        close(client_socket);
    }

    return 1;
}


void handle_connection(int client_socket){

    char buffer[512], check_buffer[512], *knock = "knock Knock\n", 
    first_part[][15] = {"Goat", "You"}, 
    second_part[][50] = {"Goat to the door and find out!\n","Yoo-hoo! Anynody home?\n"};
    USER *new_user;
    int incr = 0;

    write_to_client(client_socket, "Welcome to the server\n");

    // Get user details
    new_user = (USER*)malloc(sizeof(USER)); // Allocate memory for the structure
    get_user(new_user, client_socket); // Pass the structure by reference, and get user details

    sprintf(buffer, "Hello %s!\n", new_user->name);
    write_to_client(client_socket, buffer);
    //
    
    do{
        // Send knock knock part to the user, continue sending until satisfactory response is sent
        do{
            write_to_client(client_socket, knock);
            readLineFromNetwork(client_socket, buffer, kBufSize);
        } while (!(strcmp(buffer, "who's there?") == 0) && !(strcmp(buffer, "whos there?") == 0) && 
        !(strcmp(buffer, "whos there") == 0) && !(strcmp(buffer, "Whos there?") == 0) && !(strcmp(buffer, "Who's there?") == 0));
        //

        // Send the first part of the joke, continue sending until satisfactory response sent
        do{
            write_to_client(client_socket, first_part[incr]);
            write_to_client(client_socket, "\n");
            readLineFromNetwork(client_socket, buffer, kBufSize);
            sprintf(check_buffer, "%s who?", first_part[incr]);
            
        } while (!(strcmp(buffer, check_buffer) == 0));
        //

        // Send the final part once, as no response is needed
        write_to_client(client_socket, second_part[incr]);

        // Ask user if they want another joke, only if all the jokes have not been used
        if (incr < 1){
            write_to_client(client_socket, "Would you like another (yes/amything else)");
            readLineFromNetwork(client_socket, buffer, kBufSize);
        }

        else{
            write_to_client(client_socket, "That is all the jokes for now!\n");
        }
        //
        
    } while ((strcmp(buffer, "yes") == 0) && (++incr < 2));

    // Sign out
    sprintf(buffer, "Thank you %s, hope you enjoyed the jokes :)\n", new_user->name);
    write_to_client(client_socket, buffer);

    free(new_user->name);
    free(new_user);
    //
}

void write_to_client(int sd, char *string){

    write(sd, string, strlen(string));

}

void get_user(USER *new_user, int client_socket){

    char buffer[kBufSize];

    write_to_client(client_socket, "Please enter your name: ");
    readLineFromNetwork(client_socket, buffer, kBufSize);
    new_user->name = (char*)malloc(sizeof(char) * strlen(buffer));
    strcpy(new_user->name, buffer);

    write_to_client(client_socket, "Please enter your age: ");
    readLineFromNetwork(client_socket, buffer, kBufSize);
    new_user->age = atoi(buffer);
    
}

int readLineFromNetwork(int sd, char *buf, int size)
{	
	/* create a buffer to read into the current buffer */
	char lineBuffer[kBufSize];
	int n;
	int i, cline = 0, j = 0;

	/* raead data from network */
	/* read data into buffer */

	/* read returns how many bytes it has returned from the network */

	/* need to check we have all the data from the server */

	do
	{
	n = read(sd, lineBuffer, kBufSize);

	for (i = 0; i < n; i++)
	{	
		/* copies data into the buffer passed in */
		buf[j] = lineBuffer[i];

		/* check if youve come to the end of a line */
		if (buf[j] == 10)
		{
			buf[j] = '\0';
			/* set complete line True and exit*/
			cline = 1;
			break;
		}

		j++;
	}
	}

	/* while not complete line, if not keep reading*/
	/* check data has actually been returned */
	while(cline == 0 && n > 0);

	return n;

}
