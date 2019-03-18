#include <stdio.h>
#include <iostream>
#include <deque>  
#include <string>
#include <ctime>  

#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>

#include <iostream>
#include <thread>

#include "cards.h"

using namespace std;

void handle_client_connection(int new_socket);

int main ()
{  
   int sockfd, new_socket;
   struct sockaddr_in address; 
   int addrlen = sizeof(address);  

    // Creating socket file descriptor 
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( 5400 ); 
    // Forcefully attaching socket to the port 8080 
    if (bind(sockfd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 

    if (listen(sockfd, 200000) < 0)
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 

    cout << "Server is ready to handle client connections " << endl;

    while(1)
    {
        if ((new_socket = accept(sockfd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
        { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        }

	try
	{        // Constructs the new thread and runs it. Does not block execution.
            thread t1(handle_client_connection, new_socket);
            t1.detach();
	}
	catch(...)
	{
	    cout << "We have a critical unknown problem" << endl << flush;
            exit(EXIT_FAILURE); 
	}
    }    

    return 0; 
}

void handle_client_connection(int new_socket)
{
	char buffer[2048] = {0};

        int valread = read(new_socket ,buffer, 2048); 
        if (valread < 0)
        { 
            perror("read from client"); 
            exit(EXIT_FAILURE); 
        } 

	srand( time(0) );

   	Pack koloda;
   	koloda.shuffle();

	const char* header = "HTTP/2.0 200 OK\n" \
		"Server: vadims/1.2.1\n" \
		"Date: Sat, 08 Mar 2014 22:53:46 GMT\n" \
		"Content-Length: 1345\n" \
		"Last-Modified: Sat, 08 Jan 2018 22:53:30 GMT\n" \
		"Connection: keep-alive\n" \
		"Accept-Ranges: bytes\n" \
		"Content-type: text/html\n\n";

	string strPack = koloda.getAsString();

        if (send(new_socket, header, strlen(header) , 0) < 0)
        { 
            perror("send to client"); 
            exit(EXIT_FAILURE); 
        } 
	if (send(new_socket, strPack.c_str(),  strPack.size(), 0) < 0)
        { 
            perror("send to client"); 
            exit(EXIT_FAILURE); 
        } 
        if (close(new_socket) < 0)
        { 
            perror("close client socket"); 
            exit(EXIT_FAILURE); 
        } 
}
