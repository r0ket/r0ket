/* obreader.c by Sec <sec@42.org>
 * vim:set cin sm ts=4:
 * 
 * Do whatever you want with this code, but give credit
 *
 * This program reads packets from an USB-Serial R0ket
 * and sends them off via TCP/UDP to a central host
 *
 */

#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <pwd.h>
#include <stdlib.h>

#define BUFSIZE 100
#define PORT 2342
#define SRV_IP "127.0.0.1"

/* rotating buffer */
#define ELEMSIZE 32
unsigned int bufstart=0,bufend=0; /* rotating buffer */
#define BUFIDX(a) (a*ELEMSIZE)
#define BUFPUSH() do{ bufend++; if(bufend==BUFSIZE){ bufend=0; }; if(bufend==bufstart){ BUFPOP();}; }while(0)
#define BUFPOP() do{ bufstart++; if(bufstart==BUFSIZE){ bufstart=0; }; }while(0)
unsigned char buffer[BUFIDX(BUFSIZE)];

static u_int16_t
crc16 (const unsigned char *buffer, int size)
{
        u_int16_t crc = 0xFFFF;

        if (buffer && size)
                while (size--)
                {
                        crc = (crc >> 8) | (crc << 8);
                        crc ^= *buffer++;
                        crc ^= ((unsigned char) crc) >> 4;
                        crc ^= crc << 12;
                        crc ^= (crc & 0xFF) << 5;
                }

        return crc;
}

void setnonblocking(int fd) {
	int opts;

	opts = fcntl(fd,F_GETFL);
	if (opts < 0) {
		perror("fcntl(F_GETFL)");
		exit(EXIT_FAILURE);
	}
	opts = (opts | O_NONBLOCK);
	if (fcntl(fd,F_SETFL,opts) < 0) {
		perror("fcntl(F_SETFL)");
		exit(EXIT_FAILURE);
	}
	return;
}

/* Reference is https://r0ket.badge.events.ccc.de/tracking:reader */
void pkt_cleanup(int idx){
    static u_int32_t ctr;
    time_t t;
    time(&t);

    buffer[BUFIDX(idx)+2]=1; // BEACONLOG_SIGHTING
    buffer[BUFIDX(idx)+3]=0; // interface 0
    *(u_int16_t*)(buffer+BUFIDX(idx)+4)=htons(1234); // reader id
    *(u_int16_t*)(buffer+BUFIDX(idx)+6)=htons(32);   // size
    *(u_int32_t*)(buffer+BUFIDX(idx)+8)=htonl(ctr++);
    *(u_int32_t*)(buffer+BUFIDX(idx)+12)=htonl(t);

    *(u_int16_t*)(buffer+BUFIDX(idx)+0)=htons(0xffff ^ crc16(buffer+BUFIDX(idx)+2,30));
}

void read_r0ket(int fd){
	int r,t,o,x;
	static unsigned char data[64];
    static unsigned char offset=0;
    static unsigned char firstread=1;

	r=read(fd,data+offset,sizeof(data)-offset);
	if(r<0){
		perror("read(device)");
		exit(EXIT_FAILURE);
	};
    if(r==0){
        printf("nothing read. Shouldn't happen\n");
        return;
    };
#if 0
    for(t=offset;t<offset+r;t++){
        printf("%02x",data[t]);
    };
    printf (" read %d bytes, offset was %d\n",r,offset);
#endif
    r+=offset;
    offset=0;

    /* find frame start */
    while(data[0] != '\\' && r>0){
        if(!firstread)
            printf("ignoring garbage: %02X\n",data[0]);
        memmove(data,data+1,sizeof(data)-1);
        r--;
    };
    if(r==0){ /* no data left */
        return;
    };

    if(data[1]=='1'){
        firstread=0;
        /* find frame end */
        for(x=2;x<r-1;x++){
            if(data[x]=='\\' && data[x+1]=='0')
                break;
        }
        if(x>=r-1){ /* no EOF found */
            if(r>60){
                printf("serial frame content overflow\n");
                return;
            };
            offset=r; /* keep unused data for next round */
            return;
        };

//        printf("consume %d: ",x);
        o=16;
        BUFPUSH();
        for(t=2;t<x;t++){
            if(o>=ELEMSIZE) /* "buffer" overflow protection */
                break;
            buffer[BUFIDX(bufend)+o]=data[t];
            if(data[t]!='\\')
                o++;
//            printf("%02x",data[t]);
        };
        pkt_cleanup(bufend);
        x+=2; /* also consume end of frame marker */
//        printf("\n");
    }else if(data[1]=='7'){ /* beaconid frame */
        /* XXX: do something with beaconid */
        BUFPUSH();
        for(t=0;t<16;t++){ /* clear buffer */
            buffer[BUFIDX(bufend)+16+t]=0;
        };
        buffer[BUFIDX(bufend)+16]=22; // RFBPROTO_READER_ANNOUNCE
        *(u_int16_t*)(buffer+BUFIDX(bufend)+14)=0;
        *(u_int16_t*)(buffer+BUFIDX(bufend)+14)= \
            htons(crc16(buffer+BUFIDX(bufend),14));
        x=8;
    }else if(data[1]=='2'){ /* command ack frame */
        x=4; /* just consume, and do nothing */
    }else{
        if(!firstread)
            printf("invalid frame type: %02x\n",data[1]);
        x=2;
    };
    if(x==r) /* all data consumed */
        return;
    /* keep unconsumed data */
    memmove(data,data+x,r-x);
    offset=r-x;
	return;
}

void write_socket(int sockfd){
    BUFPOP();
    if (send(sockfd, buffer+BUFIDX(bufstart), ELEMSIZE, 0)==-1){
        perror("send");
        exit(EXIT_FAILURE);
    };
}

int main(int argc, char ** argv)
{
	int c;					/* getopt return value */
	char *device="/dev/ttyACM0";
	int devfd,sockfd;		/* FD for device & socket */
	int maxfd=0;
	int t;
	fd_set rset,wset,eset;	/* file descriptors for select() */
	struct timeval timeout;	/* Timeout for select */
    struct sockaddr_in si_other; /* target socket */

	/* The big getopt loop */
	while ((c = getopt(argc, argv, "d:")) != EOF)
		switch (c)
		{
			case 'd':
				device=(char *)malloc(strlen(optarg)+2);
				strcpy(device,optarg);
				break;

			default:
				fprintf(stderr, "Usage: %s [options] \n\n\
This program reads packets from an USB-Serial R0ket\n\
and sends them off via TCP/UDP to a central host\n\n\
   -d <device>	Open a different device instead of '%s'\n\
   -h           This help\n",
					argv[0],
					device
					);
				exit(255);
		}

/*	argc -= optind; argv += optind; *//* only if we want more args */

	/* Open & prep input device */
	if((devfd=open(device,O_RDWR)) == -1)
		perror("open_device");

	setnonblocking(devfd);

	/* Open & prep outout device */
    if ((sockfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
        perror("socket");
        exit(EXIT_FAILURE);
    };
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
        perror("inet_aton()");
        exit(EXIT_FAILURE);
    }
    if(connect(sockfd,(struct sockaddr*)&si_other,sizeof(si_other))<0){
        perror("connect");
        exit(EXIT_FAILURE);
    };
	setnonblocking(sockfd);

	/* prepare stuff for select */
	if(devfd>maxfd)
		maxfd=devfd;
	if(sockfd>maxfd)
		maxfd=sockfd;

	while(1){
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        FD_ZERO(&rset);
        FD_SET(devfd,&rset);
        FD_ZERO(&wset);
        if(bufstart!=bufend){
            FD_SET(sockfd,&wset);
        };
        FD_ZERO(&eset);
        FD_SET(devfd,&eset);

		t = select(maxfd+1, &rset, &wset, &eset, &timeout);

		if (t<0){
			perror("select");
			exit(EXIT_FAILURE);
		};

		if (t==0){ /* timeout */
			printf("[timeout]\n");
		};

		if (FD_ISSET(devfd,&rset))
			read_r0ket(devfd);

		if (FD_ISSET(sockfd,&wset))
			write_socket(sockfd);

	};
    return(0);
}
