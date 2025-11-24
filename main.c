#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>	 // for sockaddr_in
#include <arpa/inet.h> 
#include <unistd.h>      // close 

int main(){
	int 		   sfd,cfd;
	socklen_t 	   client_addr_size;
	struct sockaddr_in my_addr, client_addr;

	sfd = socket(AF_INET, SOCK_STREAM, 0);

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(8080);
	inet_pton(AF_INET, "0.0.0.0", &my_addr.sin_addr);
	bind(sfd, (struct sockaddr *) &my_addr, sizeof(my_addr));
	listen(sfd, 10);

	client_addr_size = sizeof(client_addr);
	cfd = accept(sfd, (struct sockaddr *) &client_addr, &client_addr_size);

	write(cfd, "HelloBoi\n", 10);

	char buffer[1024] = {0};
	read(cfd, buffer, 1024);
	// nc 0.0.0.0 8080 > hello 
	// curl localhost:8080/hello > ...
	printf("Buffer:%s\n",buffer); 
		
	close(sfd);
	printf("FD:%d\nSFD:%d\n",sfd,cfd);
	return 0;
}
