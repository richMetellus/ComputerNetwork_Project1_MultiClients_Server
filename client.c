/*
version 2.
This is a client that allow mutliple message to send to a provided server as a host.
This code is  base on the cs459  client_lecture with some modification to allow multiple 
connection.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>	// this file defines the structure hosten.
#include <string.h> 	// for string comp,  exit
#include <stdbool.h> 	// for boolean flag
void error(const char *msg)
{
	perror(msg);
	exit(0);
}
int main(int argc, char *argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in   serv_addr;		// struct needed to  specify the server to connect to.
	//hosten is defined in netdb.h. it defines a host computer on the interrnet
	//store information about a given host, like name, ipv4 address. 
	// only one copy of the hosten structure is allocated per thread. 
	// application should copy the info it before using the sockets api
	//i.e i thnk the sever labo2.cs.ndsu.edu and a port number.
	// have char *h_name (off name of the host) char **h_alliases alias list a NULL-terminated array of alternate names
	// int h_addrtype host address type; type of address being returned.
	// h_length: lenth in byte of each address
	struct hostent *server;
	bool exitFlag = false;

	char buffer[256];
	if (argc <3)
	{
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);
	}
	portno =  atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sockfd <0)
		error("ERROR opening socket");
	
	// the get hostby name returned the a structure fo type hosten. name is either a hostname or an IPv4 address
	// this simply copy the the name int h_name
	server = gethostbyname(argv[1]);
	if (server == NULL)
	{
		fprintf(stderr, "ERROR , no such host \n");
		exit(0);
	}

	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	//bcopy copy the information in the 1sr arg soucre and copy it to the 2nd arg, dest(which as a long 4 byte ip addr
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	
	serv_addr.sin_port = htons(portno);
	
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) <0)
		error("ERROR Connecting");
	while(!exitFlag)
	{
		printf("Please Enter The message: ");
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);
		n = write(sockfd, buffer, strlen(buffer));
		if(n<0)
			error("ERROR writing from socket");
		
		if(strcmp(buffer, "EXIT\n")== 0)
		{
			exitFlag = true;
			break;
		}
		bzero(buffer,256);
		n = read(sockfd, buffer,255);
		if(n < 0)
			printf("ERROR reading from  socket");
		printf("%s\n", buffer);
		
	}
	close(sockfd);

	return 0;
}
	
