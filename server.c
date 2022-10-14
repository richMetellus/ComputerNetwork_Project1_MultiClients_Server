

/*socketlen_t is a type that is used to declare a variable that can hold the length of a socket address, which itself is variable depending on the address family. 
It isn't that length itself/*
This server allow  multiple connection through a
specified port. This code has been adapted from the tutorial
server_lecture on blackboard.
*/

#include <stdio.h>
#include <stdlib.h>			// for IOs
#include <string.h> 
#include <unistd.h>
#include <sys/types.h> 			// for system calls
#include <sys/socket.h> 		// for sockets
#include <netinet/in.h>			// for internet 
#include <stdbool.h>
#include <pthread.h>			// for thread;

/* a function to print out error message and then abort */
void error(const char *msg)
{
	perror(msg);
	exit(1);
}

void *threadFunct(int mySockfd)
{
	char buffer2[256];	
	bool exitFlag = false;
	int read_writeLen;
	while(!exitFlag)
	{
		bzero(buffer2, 256);
 		read_writeLen = read(mySockfd, buffer2, 255);
		if (read_writeLen < 0)
			printf("ERROR reading from the client socket\n");
		
		if(strcmp(buffer2,"EXIT\n")==0)
		{
			printf("Now socket %d will be close\n", mySockfd);
			close(mySockfd);
			pthread_exit(mySockfd); 	// terminate the calling thread
		
		}
		else{
			printf("The message read from socket %d :%s\n ",mySockfd,buffer2);
			read_writeLen = write(mySockfd,"I got Your Message" , 18);
			if (read_writeLen < 0)
				printf("Unable to write to socket\n");
		}
	}
	close(mySockfd);
	return  NULL;

}
int main(int argc, char *argv[])
{
	// sockefd and  newsockfd  arfe file descriptor, array subscript
	// into the file descriptor table.
	int sockfd, newsockfd, portno;
	// 
	socklen_t clilen;  			// store the size of the address of the client..need ded for systme call
	char buffer[256];			// the server reads char from the socket connection into this buffer.
	// server_addr hold the addr of the sever &cli_adrr will contain the addr of the client
	struct sockaddr_in serv_addr, cli_addr;	// sockaddr_in is a struct use for internet communication contain ip addr and port field
							// defined in netinet/in.h
	int charRead_Written ; 				// the  number of char written or read by red() and write() calls
	
	// user need to pass in the port no on which the server will accpet connection as argument. Eroor if use fail to do that
	if(argc <2)
	{
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}
	
	//socket() create a new socket. af_inet refer to addresses(use rather PF_INET) from the internet, scok_stream is tcp
	// return a socket descriptor on sucess and -1 on error	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		error("ERROR Opening socket");
	
	// bezero sets all the values in a buffer to zero. 1st arg a pointer to a buffer
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);			// convert the port # pass as an  arg from string to digit

	serv_addr.sin_family = AF_INET;		// af_inet ipv4 domain which is usually a local host reveserve to ip addr 127.0.0.1, designate the type od addresses your  socket can commuinicate with, this case(IPv4)
	serv_addr.sin_addr.s_addr = INADDR_ANY;	// this field will contain the ip  address of the host.The INADDR_ANY  will gets this address
	serv_addr.sin_port = htons(portno);	// convert to network  byte order big ENDIAN. portno is provided by user
	
	//bind is a system call that will bind a socket to an address.
	// bind attach an ip and port to our socket.
	//return 0 on sucess and -1 on failure.
	if(bind(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	
	// another system call which allows the process to listen on the soncess a using the filde desriptor
	// the second arg is a backlog queue which set the number of connection that can be waiting while the process is handling a particular connetions
	// this call cannot fail so no need to check for error.	
	
	while(true)
	{
		pthread_t  threadId;

		listen(sockfd,10);
		clilen = sizeof(cli_addr);
	// accept() system caall wake s u p the process when a connection with client has been successful establish.
	// return a file descriptor all communiction on this connetion will be done susing the new file descriptor.
	// the second arg is a ref ptr to the address of the address of the cleint on the other end of the connection. 3rd arg size of this struc
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if(newsockfd<0)
			error("ERROR on accept");

	//remember buffer is an array of 256 char
/*		bzero(buffer, 256);
	
	// reads from the socket. it use the new file descriptor that was retruned by accept() not the original file desc  socfd.
	// read will block untill there is something to read i.e after the the client has execute a write/
	// will read teh tol num of char 255 or less
		charRead_Written = read(newsockfd, buffer, 255);
		if(charRead_Written < 0)
			error("ERROR reading from  soket");
*/
	pthread_create(&threadId,NULL,threadFunct,newsockfd);
	
	}

	close(sockfd);
	return 0;
}

