To compile this project you need to enter the following command in the unix terminal:


1) navigate to the directory where the program is located

2) Run the command for compiling
	
	for server run this: gcc server.c -o server.exe -lpthread
	
	for client run this: gcc client.c -o client.exe

3)to test the executable file run this.
	
	./server.exe [port no]; port no can be anything in the range 39000-39099
	
	./client.exe [hostname]  [port no]

4)Might need to openmany different terminal and connect the server using the same hostname where the
server is already running.

5) to exit
	exit client type in EXIT[enter], EXIT is all capital letter
	exit server: press keyboard key ctrl+C
