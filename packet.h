#ifndef PACKET_H
#define PACKET_H

#include <arpa/inet.h>
#include<cstring>

struct packet {
	uint8_t msg;
	uint8_t method;
	uint64_t key;
	uint8_t id;
	uint64_t term;
	uint64_t log_id;
};

void htonpacket(struct packet p, char buffer[27]) {
	memcpy(buffer, &p.msg, 1);
	memcpy(buffer+1, &p.method, 1);
	uint32_t u32;
	u32 = htonl(p.key >> 32);
	memcpy(buffer+2, &u32, 4);
	u32 = htonl(p.key);
	memcpy(buffer+6, &u32, 4);
	memcpy(buffer+10, &p.id, 1);
	u32 = htonl(p.term >> 32);
	memcpy(buffer+11, &u32, 4);
	u32 = htonl(p.term);
	memcpy(buffer+15, &u32, 4);
	u32 = htonl(p.log_id >> 32);
	memcpy(buffer+19, &u32, 4);
	u32 = htonl(p.log_id);
	memcpy(buffer+23, &u32, 4);
}

uint64_t deserialize(char* buffer) {
	uint32_t l,r;
	memcpy(&l, buffer, 4); 
	memcpy(&r, buffer+4, 4); 

	return ((uint64_t)ntohl(l)) << 32 | ntohl(r);
}

struct packet ntohpacket(char buffer[27]) {
	struct packet p;
	p.msg = buffer[0];
	p.method = buffer[1];
	p.key = deserialize(buffer+2);
	p.id = buffer[10];
	p.term = deserialize(buffer+11);
	p.log_id = deserialize(buffer+19);
	return p;

}

#endif