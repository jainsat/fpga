#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <csignal>
#include "packet.h"

#define BUFSIZE 27
#define HOST_PORT 3000

using namespace std;

int count = 0;

uint64_t start, end;
uint64_t cpu_time_used;
int seq[500000];

void signal_handler(int signal_num) { 
   
   // terminate program 
   cpu_time_used = end -start;
   printf("\n%lu %lu\n", start, end);
   printf("count=%d, %lu\n",count, cpu_time_used);  
//    for(int i=0;i<count;i++) {
// 	   printf("%d\n",seq[i]);
//    }
   exit(signal_num);   
}


int main(int argc, char **argv)
{
	signal(SIGINT, signal_handler); 
	struct sockaddr_in myaddr;	/* our address */
	struct sockaddr_in remaddr;	/* remote address */
	socklen_t addrlen = sizeof(remaddr);		/* length of addresses */
	int recvlen;			/* # bytes received */
	int fd;				/* our socket */
	int msgcnt = 0;			/* count # of messages we received */
	char buf[BUFSIZE];	/* receive buffer */



	/* create a UDP socket */

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket\n");
		return 0;
	}

    int32_t checkRcvBuf = 0;
    socklen_t checkRcvBufLen = sizeof(checkRcvBuf);
    getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &checkRcvBuf, &checkRcvBufLen);
    printf("socket len = %d\n",checkRcvBuf);
    int32_t new_buf = checkRcvBuf ;
    socklen_t new_buf_len = sizeof(new_buf);
    setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &new_buf, new_buf_len);
    getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &checkRcvBuf, &checkRcvBufLen);
    printf("socket len = %d\n",checkRcvBuf);

	/* bind the socket to any valid IP address and a specific port */

	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = INADDR_ANY;
	myaddr.sin_port = htons(HOST_PORT);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}

	/* now loop, receiving data and printing what we received */
	for (;;) {
		//printf("waiting on port %d %d\n", HOST_PORT, count);
		recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
		//printf("done recv\n");
		if (recvlen == 27) {
			if(count==0) {
					start =  (uint64_t)time(NULL);
			}
            struct packet p = ntohpacket(buf);
			seq[count] = p.log_id;

            count++;
			end = (uint64_t)time(NULL);

			//printf("received message: msg=%u, method=%u, key=%llu, id=%u, term=%llu, log_id=%llu, (%d bytes)\n",
             //      p.msg, p.method,p.key,p.id,p.term,p.log_id, recvlen);
            //printf("%d %llu****\n", count,p.log_id);
		}
		else {
			printf("something went wrong!\n");
			exit(1);
        }
		// sprintf(buf, "ack %d", msgcnt++);
		// printf("sending response \"%s\"\n", buf);
		// if (sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&remaddr, addrlen) < 0)
		// 	perror("sendto");
	}
	/* never exits */
    printf("done");
	return 0;
}
