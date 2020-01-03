#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h> 
#include <time.h>
#include "packet.h"


#define BUFLEN 27
#define HOST_PORT 3000


using namespace std;



int main(int argc, char *argv[])
{
	if (argc < 2) {
       fprintf(stderr,"Please provide number of msgs\n");
       exit(0);
    }
	int msgs = atoi(argv[1]);
	struct sockaddr_in myaddr, remaddr;
	int fd, i;
    socklen_t slen =sizeof(remaddr);
	char buf[BUFLEN];	/* message buffer */
	int recvlen;		/* # bytes in acknowledgement message */
    uint64_t start, end;
    uint64_t cpu_time_used;
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

    // Make a packet
    struct packet p;
	p.msg = 2;
	p.method = 1;
	p.key = 3332333;
    p.id = 9;
    p.term = 98;
    p.log_id = 9888888;

	/* now let's send the messages */

    start = (uint64_t)time(NULL);
	for (i=0; i < msgs; i++) {
		// printf("Sending packet %d to  port %d\n", i, HOST_PORT);
        p.log_id = i;
        htonpacket(p, buf); 
		if (sendto(fd, buf, BUFLEN, 0, (struct sockaddr *)&remaddr, slen)==-1) {
			perror("sendto");
			exit(1);
		}
		end = (uint64_t)time(NULL);
		//std::this_thread::sleep_for(std::chrono::milliseconds(1000));


		/* now receive an acknowledgement from the server */
		// recvlen = recvfrom(fd, buf, BUFLEN, 0, (struct sockaddr *)&remaddr, &slen);
        //         if (recvlen >= 0) {
        //                 buf[recvlen] = 0;	/* expect a printable string - terminate it */
        //                 printf("received message: \"%s\"\n", buf);
        //         }
	}
    cpu_time_used = (end - start);
    printf("\n start = %lu, end=%lu, time = %lu\n",start,end,cpu_time_used);
	close(fd);
	return 0;
}
