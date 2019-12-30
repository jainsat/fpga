#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h> 
#include "packet.h"


#define BUFLEN 2048
#define HOST_PORT 3000

#define MSGS 5	/* number of messages to send */

using namespace std;

struct Packet {
    
};

int main(void)
{
	struct sockaddr_in myaddr, remaddr;
	int fd, i;
    socklen_t slen =sizeof(remaddr);
	char buf[BUFLEN];	/* message buffer */
	int recvlen;		/* # bytes in acknowledgement message */

	/* create a UDP socket */

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket\n");
		return 0;
	}
     

	/* now define remaddr, the address to whom we want to send messages */

	memset((char *) &remaddr, 0, sizeof(remaddr));
	remaddr.sin_family = AF_INET;
	remaddr.sin_port = htons(HOST_PORT);
    remaddr.sin_addr.s_addr = INADDR_ANY;


	/* now let's send the messages */

	for (i=0; i < MSGS; i++) {
		printf("Sending packet %d to  port %d\n", i, HOST_PORT);
		sprintf(buf, "This is packet %d", i);
		if (sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&remaddr, slen)==-1) {
			perror("sendto");
			exit(1);
		}
		/* now receive an acknowledgement from the server */
		// recvlen = recvfrom(fd, buf, BUFLEN, 0, (struct sockaddr *)&remaddr, &slen);
        //         if (recvlen >= 0) {
        //                 buf[recvlen] = 0;	/* expect a printable string - terminate it */
        //                 printf("received message: \"%s\"\n", buf);
        //         }
	}
	close(fd);
	return 0;
}
