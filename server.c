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


void handle_connection(int);
int readLineFromNetwork(int sd, char *buf, int size);
void write_to_client(int sd, char *buf);


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

    // check for range of the port
    if (!(server_port >= 1024 && server_port <= 65536)){
        fprintf(stderr, "{ERROR} Server port out of range\n");
        exit(1);
    }

    // create the socket type internet, stream of data, and tcp protocol
    if ((server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
        
        fprintf(stderr, "{ERROR} Cannot create the socket\n");
        exit(1);
    } 

    // create the server address
    memset(&server_addr, 0, sizeof(server_addr)); // set all data to 0, as some will not be used
    server_addr.sin_family = AF_INET; // set type of connection to be internet
    server_addr.sin_addr.s_addr = INADDR_ANY; // set any ip address to be listening to connection   
    server_addr.sin_port = htons(server_port); // convert to big endian number which is network standard

    //bind the socket to the port and the ip address

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){

        fprintf(stderr, "{ERROR} Socket could not be binded\n");
        exit(1);
    }

    // listen to incomming connections
    listen(server_socket, 10); //listen on the server socket, and allow 10 machines to queue

    // accept the connections and handle
    while(1){
        
        client_length = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_length);

        handle_connection(client_socket);

        close(client_socket);
    }

    return 1;
}


void handle_connection(int client_socket){

    char buffer[512];
    char *knock = "knock Knock\n";
    char first_part[][15] = {"Goat\n", "You\n"}, second_part[][50] = {"Goat to the door and find out!\n","Yoo-hoo! Anynody home?\n"};
    

    int length_check, inc = 0;

    write_to_client(client_socket, "Welcome to the server\n");

    do{
  
        do{
            write_to_client(client_socket, knock); //knock knock
            bzero(buffer, strlen(buffer));
            read(client_socket, buffer, 512);
        }while (!(strcmp(buffer, "who's there\n") == 0));

       
        write_to_client(client_socket, first_part[inc]); // first part

        bzero(buffer, strlen(buffer));
        read(client_socket, buffer, 512);

        write_to_client(client_socket, second_part[inc]); // second part

        write_to_client(client_socket, "Would you like another joke? (yes for another)"); // second part

        bzero(buffer, strlen(buffer));
        read(client_socket, buffer, 512);
        



    }
    
    while((strcmp(buffer, "yes\n") == 0) && (inc++ < 2));

    write_to_client(client_socket, "Goodbye :)");
    
}



void write_to_client(int sd, char *buffer){

    
    write(sd, buffer, strlen(buffer));

}

