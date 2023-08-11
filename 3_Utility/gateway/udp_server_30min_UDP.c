#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>

#define SERV_PORT 59478

#define MAXNAME 1024

extern int errno;

int main(){
	int socket_fd; /* file description into transport */
	int srv_length; /* server length of address structure */
	int nbytes; /* the number of read **/
	char buf[BUFSIZ];
	struct sockaddr_in myaddr; /* address of this service */
	struct sockaddr_in client_addr; /* address of client */
	int total_rec_bytes= {0};
	FILE *source,*goal, *datafail;
	struct timeval tv1;
  	struct timezone tz1;
	long temp_time_usec = 0;
	long temp_time_sec = 0;
	int count = 0;
	int count1 = 0; 
	long max_delay = 0;
	int payload_lenth_addr = 16;
	int payload_addr = 18;
	int message_ID_addr = 12;
	int bus_ID_addr = 8;
	int UDP_start = 0;
	int message_ID[6][101] = {0};
	long message_Time_total[6][101] = {0};
	long message_Time_max[6][101] = {0};
	
	struct message_rec_Time
	{
		long message_Time_sec;
		long message_Time_usec;
		long message_Time_msec;
	};
	struct message_rec_Time temp_time[6][101] = {0};

	
	/*
	* Get a socket into UDP/IP
	*/
	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) <0) {
		perror ("socket failed");
		exit(EXIT_FAILURE);
	}

	/*
	* Set up our address
	*/
	bzero ((char *)&myaddr, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(SERV_PORT);

	/*
	* Bind to the address to which the service will be offered
	*/
	if (bind(socket_fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) <0) {
		perror ("bind failed\n");
		exit(1);
	}

	/*
	* Loop continuously, waiting for datagrams
	* and response a message
	*/
	srv_length = sizeof(client_addr);
	printf("Server is ready to receive !!\n");
	printf("Can strike Cntrl-c to stop Server >>\n");
	while (1) {
		if ((nbytes = recvfrom(socket_fd, &buf, MAXNAME, 0, (struct sockaddr*)&client_addr, (socklen_t *)&srv_length)) <0) {
			perror ("could not read datagram!!");
			continue;
		}
		
		total_rec_bytes = total_rec_bytes + nbytes;
		/*printf("the length of received UDP is %d bytes:\r\n", nbytes);
		for (int i=0; i<16; i++) {
			printf(" %02d  ", i);
		}
		printf("\r\n");
		for (int i=0; i<nbytes; i++) {
			printf("0x%02X ", buf[i]);
			if(i%16==15)printf("\r\n");
		}
		printf("\r\n");
		printf("Received data form %s : %d\n", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));
		printf("%s\n", buf);
		
		/* return to client */
		// gettimeofday (&tv1 , &tz1);
		// while(UDP_start+buf[payload_lenth_addr]+18<=nbytes){
		/*printf("--------------------------------------\r\n");
		printf("UDP_start:0x%02d\r\n ", UDP_start);
		printf("payload_lenth_addr:0x%02d\r\n ", payload_lenth_addr);
		printf("payload_addr:0x%02d\r\n ", payload_addr);
		printf("message_ID_addr:0x%02d\r\n ", buf[message_ID_addr]);
		printf("buf[payload_lenth_addr]:0x%02d\r\n ", buf[payload_lenth_addr]);
		*/
		
			// for(int i = payload_addr;i<payload_addr+buf[payload_lenth_addr];i++)
			// {
			// 	int a = i - payload_addr;
			// 	//printf("DATA %d:%d\r\n ", i,buf[i]);
				
			// 	if(buf[i]!=161+a)
			// 	{
			// 		datafail=fopen("datafail.txt","a");
			// 		fprintf(datafail, "Data : %d \r\n",buf[i]);
			// 		fclose(datafail);
			// 	}
				
			// }
		// 	message_ID[buf[bus_ID_addr]][buf[message_ID_addr]]++;
		// 	if(temp_time[buf[bus_ID_addr]][buf[message_ID_addr]].message_Time_msec == 0)
		// 	{
		// 		//printf("------------\r\n");
		// 		temp_time[buf[bus_ID_addr]][buf[message_ID_addr]].message_Time_sec = tv1.tv_sec;
		// 		temp_time[buf[bus_ID_addr]][buf[message_ID_addr]].message_Time_usec = tv1.tv_usec;
		// 		temp_time[buf[bus_ID_addr]][buf[message_ID_addr]].message_Time_msec = tv1.tv_sec*1000 + tv1.tv_usec/1000;
		// 		// printf("sec %d:%ld\r\n ",buf[message_ID_addr],temp_time[buf[message_ID_addr]].message_Time_sec);
		// 		// printf("usec %d:%ld\r\n ",buf[message_ID_addr],temp_time[buf[message_ID_addr]].message_Time_usec);
		// 		// printf("msec:%d:%ld\r\n ",buf[message_ID_addr],temp_time[buf[message_ID_addr]].message_Time_msec);
		// 	}
		// 	else
		// 	{
		// 		long temp_msec = tv1.tv_sec*1000 + tv1.tv_usec/1000;

		// 		message_Time_total[buf[bus_ID_addr]][buf[message_ID_addr]] = message_Time_total[buf[bus_ID_addr]][buf[message_ID_addr]] + temp_msec-temp_time[buf[bus_ID_addr]][buf[message_ID_addr]].message_Time_msec;
		// 		//printf("diff %d:%ld\r\n ",buf[message_ID_addr],temp_msec-temp_time[buf[message_ID_addr]].message_Time_msec);
		// 		//printf("total %d:%ld\r\n ",buf[message_ID_addr],message_Time_total[buf[message_ID_addr]]);
		// 		if(temp_msec-temp_time[buf[bus_ID_addr]][buf[message_ID_addr]].message_Time_msec>message_Time_max[buf[bus_ID_addr]][buf[message_ID_addr]])
		// 		{
		// 			message_Time_max[buf[bus_ID_addr]][buf[message_ID_addr]] = temp_msec-temp_time[buf[bus_ID_addr]][buf[message_ID_addr]].message_Time_msec;
		// 			//printf("max %d:%ld\r\n ",buf[message_ID_addr],message_Time_max[buf[message_ID_addr]]);
		// 		}
				
		// 		temp_time[buf[bus_ID_addr]][buf[message_ID_addr]].message_Time_sec = tv1.tv_sec;
		// 		temp_time[buf[bus_ID_addr]][buf[message_ID_addr]].message_Time_usec = tv1.tv_usec;
		// 		temp_time[buf[bus_ID_addr]][buf[message_ID_addr]].message_Time_msec = temp_msec;
		// 		// printf("+++++++++++++++++++++\r\n");
		// 		// printf("sec %d:%ld\r\n ",buf[message_ID_addr],temp_time[buf[message_ID_addr]].message_Time_sec);
		// 		// printf("usec %d:%ld\r\n ",buf[message_ID_addr],temp_time[buf[message_ID_addr]].message_Time_usec);
		// 		// printf("msec:%d:%ld\r\n ",buf[message_ID_addr],temp_time[buf[message_ID_addr]].message_Time_msec);
		// 	}
				
			
			
		// 	UDP_start = payload_lenth_addr + buf[payload_lenth_addr]+2;
		// 	payload_lenth_addr = UDP_start + 16;
		// 	payload_addr = payload_lenth_addr+2;
		// 	message_ID_addr = UDP_start + 12;
		// 	bus_ID_addr = UDP_start + 8;
		// }
		// payload_lenth_addr = 16;
		// message_ID_addr = 12;
		// UDP_start = 0;
		// payload_addr = 18;
		// bus_ID_addr = 8;
		/*for(int i=0;i<66;i++)
		{
			printf("message ID %d:%d\r\n ", i,message_ID[i]);
		}*/
		if (sendto(socket_fd, &buf, nbytes, 0, (struct sockaddr*)&client_addr, srv_length) < 0) {
			perror("Could not send datagram!!\n");
			continue;
		}		
		// if(temp_time_sec==0)
		// {
		// 	// temp_time_sec = tv1.tv_sec;
		// 	// temp_time_usec = tv1.tv_usec;
		// 	// count1++;
		// 	goal=fopen("result.txt","a"); //開啟一個要寫入的檔案 
		// 	fprintf(goal, "Time : %ld.%ld \r\n", tv1.tv_sec,tv1.tv_usec);
		// 	fprintf(goal, "Minus : %ld\r\n", temp_time_usec);
		// }
		// else
		// {
			// long a ;
			// if(tv1.tv_sec>temp_time_sec)
			// {
			// 	long temp = (tv1.tv_sec-temp_time_sec)*1000000;
			// 	a = tv1.tv_usec + temp - temp_time_usec;

			// }
			// else
			// {
			// 	a = tv1.tv_usec-temp_time_usec;
				
			// }
			// temp_time_sec = tv1.tv_sec;
			// temp_time_usec = tv1.tv_usec;
			// if(a > 5000)
			// {
			// 	if(a>max_delay){
			// 		max_delay = a;
			// 	}
			// 	count++;
			// 	source=fopen("resultfail.txt","a");
			// 	fprintf(source, "Time : %ld.%06ld \r\n", tv1.tv_sec,tv1.tv_usec);
			// 	fprintf(source, "Minus : %ld\r\n", a);
			// 	fprintf(source, "Count : %d\r\n", count);
			// 	fprintf(source, "Max : %ld\r\n", max_delay);
			// 	fclose(source);
			// }
			// count1++;
			// goal=fopen("result.txt","a"); //開啟一個要寫入的檔案 
			// fprintf(goal, "Time : %ld.%06ld \r\n", tv1.tv_sec,tv1.tv_usec);
			// fprintf(goal, "Minus : %ld\r\n", a);
			
			
		// }
		// if(message_ID[0][4]>140000)
		// {
			// int total[6]={0};
			// for(int j = 0;j<6;j++)
			// {
			// 	for(int i=0;i<101;i++)
			// 	{
			// 		total[j] = total[j]+message_ID[j][i];
			// 		fprintf(goal,"Can %d message ID %d:%d\r\n ",j, i,message_ID[j][i]);
			// 		fprintf(goal,"Can %d message %d average tranfer time :%.2f\r\n ",j, i,(float)message_Time_total[j][i]/(message_ID[j][i]-1));
			// 		fprintf(goal,"Can %d message %d maximum tranfer time :%ld\r\n ",j, i,message_Time_max[j][i]);
			// 	}
			// 	fprintf(goal, "Can %d Total received message  %d :\r\n",j, total[j]);
			// }
			goal=fopen("result_udpbytes.txt","a");
			fprintf(goal, "Total received UDP is %d bytes:\r\n", total_rec_bytes);
				//fprintf(goal, "Total received UDP  %d :\r\n", total);
			// fprintf(goal, "Count : %d\r\n", count1);
			fclose(goal);
		// }
		// else
		// {
			// fclose(goal);
		// }
		
		
		
	}
	
	return 0;
}

