#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <string.h>
#include <arpa/inet.h>

#define SERV_PORT 59478
#define MAXNAME 1024

#define PACKET_HEADER_DIR_X2A      0x14
#define PACKET_HEADER_DIR_A2X      0x28
#define PACKET_HEADER_SID_MA       0x01
#define PACKET_HEADER_SID_AA       0x02
#define PACKET_HEADER_SID_XA       0x03
#define PACKET_HEADER_SID_XB       0x04
#define PACKET_HEADER_TID_MA       0x01
#define PACKET_HEADER_TID_AA       0x02
#define PACKET_HEADER_TID_XA       0x03
#define PACKET_HEADER_TID_XB       0x04
#define PACKET_HEADER_TYPE_CAN     0x00
#define PACKET_HEADER_TYPE_ERAY    0x02
#define PACKET_BUSID_CANA          0x00
#define PACKET_BUSID_CANB          0x01
#define PACKET_BUSID_CANC          0x02
#define PACKET_BUSID_CAND          0x03
#define PACKET_BUSID_CANE          0x04
#define PACKET_BUSID_CANF          0x05

extern int errno;
uint get_cnt = 0;

int GetLittleEndianIntegerFromCharArray(char *data, int length)
{
    int result = 0;
    for (int i = length - 1; i >= 0; i--) {
        result |= data[i] << i * 8;
    }
    return result;
}

void parseFrame(char* buf, int nbytes)
{
    FILE *outfp;
    outfp = fopen("udptest.log","a");
    while(nbytes > 0) {

        char packet_haeder_dir  = buf[0];
        char packet_haeder_sid  = buf[1];
        char packet_haeder_tid  = buf[2];
        char packet_haeder_type = buf[3];

        // check header
        if (packet_haeder_dir != PACKET_HEADER_DIR_X2A &&
            packet_haeder_dir != PACKET_HEADER_DIR_A2X) {
            fprintf(outfp, "\nNot UDP Frame format\n");
            return;
        }
        if (packet_haeder_sid != PACKET_HEADER_SID_MA &&
            packet_haeder_sid != PACKET_HEADER_SID_AA &&
            packet_haeder_sid != PACKET_HEADER_SID_XA &&
            packet_haeder_sid != PACKET_HEADER_SID_XB) {
            fprintf(outfp, "\nNot UDP Frame format\n");
        }
        if (packet_haeder_tid != PACKET_HEADER_TID_MA && 
            packet_haeder_tid != PACKET_HEADER_TID_AA && 
            packet_haeder_tid != PACKET_HEADER_TID_XA && 
            packet_haeder_tid != PACKET_HEADER_TID_XB) {
            fprintf(outfp, "\nNot UDP Frame format\n");
        }
        if (packet_haeder_type != PACKET_HEADER_TYPE_CAN && 
            packet_haeder_type != PACKET_HEADER_TYPE_ERAY) {
            fprintf(outfp, "\nNot UDP Frame format\n");
        }
        // get size
        uint size = 0;
        size = GetLittleEndianIntegerFromCharArray(&buf[4], 2);
        if (size > nbytes - 6) {
            fprintf(outfp, "Error size, %d, %d\n", size, nbytes - 6);
        }
        // get data
        char *data;
        data = &buf[6];
        {
            get_cnt ++;
            fprintf(outfp, "****** Get UDP frame, Cnt: [%d] ******\n",get_cnt);
            uint info_code = GetLittleEndianIntegerFromCharArray(&data[0], 2);
            char bus_id = data[2];
            char rsv_1 = data[3];
            char rsv_2 = data[4];
            char rsv_3 = data[5];
            uint message_id = GetLittleEndianIntegerFromCharArray(&data[6], 4);
            uint payload_length = GetLittleEndianIntegerFromCharArray(&data[10], 2);
            char *payload;
            payload = &data[12];

            // info code
            fprintf(outfp, "INFO: %04X\n",info_code);
            // Bus ID
            if (packet_haeder_type == PACKET_HEADER_TYPE_CAN) {
                fprintf(outfp, "CAN Bus ID: ");
                switch(data[2]){
                    case PACKET_BUSID_CANA:
                        fprintf(outfp, "Node_A\n");
                        break;
                    case PACKET_BUSID_CANB:
                        fprintf(outfp, "Node_B\n");
                        break;
                    case PACKET_BUSID_CANC:
                        fprintf(outfp, "Node_C\n");
                        break;
                    case PACKET_BUSID_CAND:
                        fprintf(outfp, "Node_D\n");
                        break;
                    case PACKET_BUSID_CANE:
                        fprintf(outfp, "Node_E\n");
                        break;
                    case PACKET_BUSID_CANF:
                        fprintf(outfp, "Node_F\n");
                        break;
                    default:
                        fprintf(outfp, "Error node!!\n");
                        return;
                }
            }
            // Message ID
            fprintf(outfp, "Message ID: %d\n",message_id);
            // Print payload
            fprintf(outfp, "Payload Data: \n");
            for (int i = 0; i < payload_length; i++) {
                fprintf(outfp, "0x%02X ", payload[i]);
                if(i % 16 == 15) fprintf(outfp, "\n");
            }
            fprintf(outfp, "\n");
        }
        nbytes = nbytes - 6 - size;
        if (nbytes > 0) {
            buf = buf + size + 6;
        }
    }
    fclose(outfp);
}

void rawdataLog(char* buf, int nbytes)
{
    FILE *outfp;
    outfp = fopen("udpraw.log","a");
    fprintf(outfp, "the length of received UDP is %d bytes:\n", nbytes);
    for (int i = 0; i < nbytes; i++) {
        fprintf(outfp, "0x%02X ", buf[i]);
        if(i % 16 == 15)fprintf(outfp, "\n");
    }
    fprintf(outfp, "\n");
    fclose(outfp);
}

int main(){
    int socket_fd; /* file description into transport */
    int srv_length; /* server length of address structure */
    int nbytes; /* the number of read **/
    char buf[BUFSIZ];
    struct sockaddr_in myaddr; /* address of this service */
    struct sockaddr_in client_addr; /* address of client */

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
    while (1) {
        if ((nbytes = recvfrom(socket_fd, &buf, MAXNAME, 0, (struct sockaddr*)&client_addr, (socklen_t *)&srv_length)) < 0) {
            perror ("could not read datagram!!");
            continue;
        }
        // rawdataLog(buf, nbytes);
        parseFrame(buf, nbytes);
    }
    return 0;
}

