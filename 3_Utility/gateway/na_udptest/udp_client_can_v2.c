#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>

#define SERV_PORT 59478

void hton_16(unsigned char x[4], unsigned short v) {
    x[0] = (v & 0xFF);
	x[1] = (v >> 8) & 0xFF;
}

void hton_32(unsigned char x[4], unsigned int v) {
    x[0] = (unsigned char) (v);
    x[1] = (unsigned char) (v >> 8);
    x[2] = (unsigned char) (v >> 16);
    x[3] = (unsigned char) (v >> 24);
}

void print_udp(unsigned char data[], int udpLength) {
	printf("the length of sending UDP is %d bytes:\r\n", udpLength);
	for (int i=0; i<16; i++) {
		printf(" %02d  ", i);
	}
	printf("\r\n");
	for (int i=0; i<udpLength; i++) {
		printf("0x%02X ", data[i]);
		if(i%16==15)printf("\r\n");
	}
	printf("\r\n\r\n");
}

int main(int argc, char **argv){
	int fd;     /* fd into transport provider */
	int size;    /* the length of servaddr */
	int udpHeader = 18;
	int canLength;
	int udpLength;
	struct hostent *hp;   /* holds IP address of server */
	struct sockaddr_in myaddr;   /* address that client uses */
	struct sockaddr_in servaddr; /* the server's full addr */

	canLength = strtol(argv[5], NULL, 0);
	udpLength = udpHeader + canLength;
	unsigned char data[udpLength];

	memset(data, 0, sizeof(data));

	/* data[1-2]: source/target ID */
	for(int m=1; m<3; m++) {
		if (!strcmp(argv[m], "AA")) {
			data[m] = 1;
		} else if (!strcmp(argv[m], "AB")) {
			data[m] = 2;
		} else if (!strcmp(argv[m], "XA")) {
			data[m] = 3;
		} else if (!strcmp(argv[m], "XB")) {
			data[m] = 4;
		} else {
			printf("error source ID or target ID\r\n");
			return 0;
		}
	}
	
	/* data[0]: packet directoin */
	if ((data[1] == 1 || data[1] == 2) && (data[2] == 3 || data[2] == 4)) {
		data[0] = 0x28;
	} else if ((data[1] == 3 || data[1] == 4) && (data[2] == 1 || data[2] == 2)) {
		data[0] = 0x14;
	} else {
		printf("error packet direction\r\n");
		return 0;
	}
	
	/* data[4-5]: info length */
	hton_16(&data[4], 12+ canLength);

	/* data[8]: bus ID */
	if (argv[3][0] == 'a' || argv[3][0] == 'A') {
		data[8] = 0;
	} else if (argv[3][0] == 'b' || argv[3][0] == 'B') {
		data[8] = 1;
	} else if (argv[3][0] == 'c' || argv[3][0] == 'C') {
		data[8] = 2;
	} else if (argv[3][0] == 'd' || argv[3][0] == 'D') {
		data[8] = 3;
	} else if (argv[3][0] == 'e' || argv[3][0] == 'E') {
		data[8] = 4;
	} else if (argv[3][0] == 'f' || argv[3][0] == 'F') {
		data[8] = 5;
	} else {
		printf("error CAN BUS number\r\n");
		return 0;
	}

	/* data[12-15]: filter ID */
	hton_32(&data[12], strtol(argv[4], NULL, 0));

	/* data[16-17]: length */
	hton_16(&data[16], canLength);

	if (canLength == 0) {
		printf("canLength is 0\r\n");
	} else if (!(canLength%4)) {
		for(int m = 0; m < canLength/4; m++) {
			hton_32(&data[udpHeader + m*4], strtol(argv[6+m], NULL, 0));
		}	
	} else {
		if (canLength < 1 && canLength > 7) {
			printf("error canData length\r\n");
			return 0;
		}
		int idx = 0;
		if ((canLength/4) == 1) {
			hton_32(&data[udpHeader], strtol(argv[6], NULL, 0));
			idx++;
			canLength -= 4;
		}
		if (canLength == 1) {
			data[udpHeader + idx*4] = strtol(argv[6+idx], NULL, 0); 
		} else if (canLength == 2) {
			hton_16(&data[udpHeader + idx*4], strtol(argv[6+idx], NULL, 0));
		} else if (canLength == 3) {
			int value = strtol(argv[6+idx], NULL, 0);
			data[udpHeader + idx*4] = value & 0xFF;
			data[udpHeader + idx*4 + 1] = (value>>8) & 0xFF;
			data[udpHeader + idx*4 + 2] = (value>>16) & 0xFF;
			hton_16(&data[udpHeader + idx*4], strtol(argv[6+idx], NULL, 0));
		}
	}
	print_udp(data, udpLength);
	
	/*
	*  Get a socket into UDP
	*/
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror ("socket failed!");
		exit(1);
	}

	/*
	* Bind to an arbitrary return address.
	*/
	bzero((char *)&myaddr, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(0);

	if (bind(fd, (struct sockaddr *)&myaddr,
		sizeof(myaddr)) <0) {
		perror("bind failed!");
		exit(1);
	}

	/*
	* Fill in the server's UDP/IP address
	*/
	bzero((char *)&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT); 
	if (data[2] == 1) {
		hp = gethostbyname("10.42.0.146");
	} else if (data[2] == 2) {
		hp = gethostbyname("10.42.0.147");
	} else if (data[2] == 3) {
		hp = gethostbyname("10.42.0.28");
	}  else if (data[2] == 4) {
		hp = gethostbyname("10.42.0.29");
	} 

	if (hp == 0) {
		fprintf(stderr, "could not obtain address of %s\n", argv[2]);
		return (-1);
	}

	bcopy(hp->h_addr_list[0], (caddr_t)&servaddr.sin_addr,
			hp->h_length);


	/* send data to Server */
	size = sizeof(servaddr);
	if (sendto(fd, data, udpLength, 0, (struct sockaddr*)&servaddr, size) == -1) {
		perror("write to server error !");
		exit(1);
	}
	return 0;
}


