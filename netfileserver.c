#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 


int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025];
    time_t ticks; 

// 1) The call to the function ‘socket()’ creates an UN-named socket inside the kernel and returns an integer known as socket descriptor. 
//This function takes domain/family as its first argument. For Internet family of IPv4 addresses we use AF_INET.	
//The second argument ‘SOCK_STREAM’ specifies that the transport layer protocol that we want should be reliable ie it should have acknowledgement techniques. For example : TCP
	
    listenfd = socket(AF_INET, SOCK_STREAM, 0);	
	
//	The third argument is generally left zero to let the kernel decide the default protocol to use for this connection. For connection oriented reliable connections, the default protocol used is TCP.	
// 1 ----	END OF STEP 1
	
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

	
// 2)	The call to the function ‘bind()’ assigns the details specified in the structure ‘serv_addr’ to the socket created in the step 1. The details include, the family/domain, the interface to listen on(in case the system has multiple interfaces to network) and the port on which the server will wait for the client requests to come.	
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

// 2 ----	END OF STEP 2
	
// 3) The call to the function ‘listen()’ with second argument as ’10’ specifies maximum number of client connections that server will queue for this listening socket.
// After the call to listen(), this socket becomes a fully functional listening socket	
	
    listen(listenfd, 10); 

// 3 ----	END OF STEP 3
	
    while(1)
    {
		
// 4)	In the call to accept(), the server is put to sleep and when for an incoming client request, the three way TCP handshake* is complete, the function accept () wakes up and returns the socket descriptor representing the client socket.
//The call to accept() is run in an infinite loop so that the server is always running and the delay or sleep of 1 sec ensures that this server does not eat up all of your CPU processing.
//As soon as server gets a request from client, it prepares the date and time and writes on the client socket through the descriptor returned by accept().		
		
		
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

// 4 ----	END OF STEP 4		
		
        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff)); 

        close(connfd);
        sleep(1);
     }
}