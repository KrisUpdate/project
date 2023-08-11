#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>       // time_t, time (for timestamp in second)
#include <sys/timeb.h>  // ftime, timeb (for timestamp in millisecond)
#include <sys/time.h>   // gettimeofday, timeval (for timestamp in microsecon
#include <signal.h>

#define SERV_PORT 59478

#define MAXNAME 2048


extern int errno;
unsigned long long nbytes_total;
unsigned long long nbytes_a;
unsigned long long nbytes_b;
unsigned long long nbytes_c;
unsigned long long nbytes_d;
unsigned long long nbytes_e;
unsigned long long nbytes_f;
void sigroutine(int dunno) {
  switch (dunno) {
    case 1:
        printf("Get a signal -- SIGHUP \r\n");
        break;
    case 2:
        printf("Get a signal -- SIGINT \r\n");
        printf("nbytes total = %lld bytes\r\n", nbytes_total);
        printf("nbytes a = %lld bytes\r\n", nbytes_a);
        printf("nbytes b = %lld bytes\r\n", nbytes_b);
        printf("nbytes c = %lld bytes\r\n", nbytes_c);
        printf("nbytes d = %lld bytes\r\n", nbytes_d);
        printf("nbytes e = %lld bytes\r\n", nbytes_e);
        printf("nbytes f = %lld bytes\r\n", nbytes_f);
	//Ctrl + C
        break;
    case 3:
        printf("Get a signal -- SIGQUIT \r\n");
        break;
   }
   exit(0);
}

int GetLittleEndianIntegerFromCharArray(char *data, int length)
{
    int result = 0;
    for (int i = length - 1; i >= 0; i--) {
      result |= data[i] << i * 8;
    }
    return result;
}

int main(){
	nbytes_total = 0;
	nbytes_a = 0;
	nbytes_b = 0;
	nbytes_c = 0;
	nbytes_d = 0;
	nbytes_e = 0;
	nbytes_f = 0;
	signal(SIGINT, sigroutine);
	int socket_fd;                                           
	int srv_length;                                         
	int nbytes; 
	int udpLength = 0;
	char buf[BUFSIZ];
	struct sockaddr_in myaddr; 
	struct sockaddr_in client_addr; 
	struct timeval timer_usec; 
	long long int timestamp_usec; 
	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) <0) {
		perror ("socket failed");
		exit(EXIT_FAILURE);
	}
	bzero ((char *)&myaddr, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(SERV_PORT);
        if (bind(socket_fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) <0) {
		perror ("bind failed\n");
		exit(1);
	}
        srv_length = sizeof(client_addr);
	printf("Server is ready to receive !!\n");
	printf("Can strike Cntrl-c to stop Server >>\n");
	while(1){
		if ((nbytes = recvfrom(socket_fd, &buf, MAXNAME, 0, (struct sockaddr*)&client_addr, (socklen_t *)&srv_length)) <0) {
			perror ("could not read datagram!!");
			continue;
		}
	        nbytes_total += nbytes;	
		//
		//if (!gettimeofday(&timer_usec, NULL)) {
		//  timestamp_usec = ((long long int) timer_usec.tv_sec) * 1000000ll + (long long int) timer_usec.tv_usec;
		//  printf("received %d bytes UDP at %lld.%06lld secs\r\n", nbytes, (long long int)timer_usec.tv_sec, (long long int)timer_usec.tv_usec);
		//}
		//
    udpLength = 18+ buf[16];
    uint size = 0;
    for (int i=0; i<nbytes; i+=udpLength) {
      size = GetLittleEndianIntegerFromCharArray(&buf[i+4], 2);
      //printf("size: %d\r\n", size);
      char *data;
      data = &buf[i+6];
      uint message_id;
      uint payload_length;
      message_id = GetLittleEndianIntegerFromCharArray(&data[6], 4);
      payload_length = GetLittleEndianIntegerFromCharArray(&data[10], 2);
      switch(buf[i+8]){
        case 0x00:
          nbytes_a += udpLength;
          //printf("CAN_A: id=%d,payload_len=%d\r\n",message_id,payload_length);
          break;
        case 0x01:
          nbytes_b += udpLength;
          //printf("CAN_B: id=%d,payload_len=%d\r\n",message_id,payload_length);
          break;
        case 0x02:
	      nbytes_c += udpLength;
          //printf("CAN_C: id=%d,payload_len=%d\r\n",message_id,payload_length);
          break;
	    case 0x03:
	      nbytes_d += udpLength;
          //printf("CAN_D: id=%d,payload_len=%d\r\n",message_id,payload_length);
          break;
        case 0x04:
	      nbytes_e += udpLength;
          //printf("CAN_E: id=%d,payload_len=%d\r\n",message_id,payload_length);
          break;
	    case 0x05:
	      nbytes_f += udpLength;
          //printf("CAN_F: id=%d,payload_len=%d\r\n",message_id,payload_length);
          break;
	default:
	  break;
      }
      //printf("0x%02X, 0x%02X\r\n", buf[i+8], buf[i+udpLength-1]);
      if (i+udpLength < nbytes) {
        udpLength = 18 + buf[i + udpLength + 16];
      }
     }
   }
	return 0;
}
