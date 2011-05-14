
#include <fonts.h>
#include <render.h>

#define MAXCHR (24*3)
static uint8_t buf[MAXCHR];

uint8_t * pk_decode(const uint8_t * data,int*len){
//	int off=0;				// Offset into au8FontTable for bytestream
	int length=*len;		// Length of character bytestream
	int height;				// Height of character in bytes
	uint8_t * bufptr=buf;	// Output buffer for decoded character

	height=(font->u8Height-1)/8+1;

	// Local function: Get next nibble.
	int ctr=0;  // offset for next nibble
	int hilo=0; // 0= high nibble next, 1=low nibble next
	char gnn(){ // Get next nibble
		static int byte;
		int val;
		hilo=1-hilo;
		if(hilo==1){
			byte=data[ctr];
			ctr++;
			val=byte>>4;
		}else{
			val=byte&0x0f;
		};
		return val;
	};

	// Local function: Unpack "long run".
	int upl(int off){ // unpack "long" run.
		int retval;

		while((retval=gnn())==0){
			off++;
		};
		while(off-->0){
//			printf("- rv: %x\n",retval);
			retval=retval<<4;
			retval+=gnn();
		};
//			printf("> rv: %x\n",retval);
		return retval;
	};

	if(data[0]>>4 == 15){
		// This is a raw character!
		int preblank,postblank;
		const uint8_t *rawptr;
		preblank=  data[1];
		postblank= data[2];
		rawptr=&data[3];
//		printf("Raw character: pre=%d, post=%d\n",preblank,postblank);
		return;
	};


#define DNY (12)  // Decoder parameter: Fixed value for now.
	int repeat=0; // Decoder internal: repeat colum?
	int curbit=0; // Decoder internal: current bit (1 or 0)
	int pos=0;	  // Decoder internal: current bit position (0..7)
	int nyb;      // Decoder internal: current nibble / value

	while(ctr<length){ /* Iterate the whole input stream */

		/* Get next encoded nibble and decode */
		nyb=gnn();

//		printf("\nCtr: %d, byte: %x, Process: %d\n",ctr, data[ctr], nyb);

		if(nyb==15){
			repeat++;
			continue;
		}else if(nyb==14){
			nyb=upl(0);
			nyb+=1;
//			printf("14-decode: %d\n",nyb);
			repeat+=nyb;
			continue;
		}else if(nyb>DNY){
			nyb=(16*(nyb-DNY-1))+gnn()+DNY+1;
		}else if(nyb==0){
			nyb=upl(1);
			nyb+=(16*(13-DNY)+DNY)-16;
//			printf("0-decode: %d\n",nyb);
		};

		/* Generate & output bits */

//		printf("have %d bits. Got %d (%d)-bits...(r=%d)",pos, nyb, curbit,repeat);
		while(nyb-->0){
			if(curbit==1){
				*bufptr|=1<<(7-pos);
			}else{
				*bufptr&=~(1<<(7-pos));
			};
			pos++;
			if(pos==8){
//				printf("BYTE: 0x%02x\n",*bufptr);
				bufptr++;
				if((bufptr-buf)%height==0){ // End of column?
					while(repeat>0){
						for(int y=0;y<height;y++){
							bufptr[0]=bufptr[-3];
							bufptr++;
						};
//						printf("repeated last line\n");
						repeat--;
					};
				};
				pos=0;
			};
		};
		curbit=1-curbit;
	};
		
//	printf("\n");

	*len=(bufptr-buf)/height; // return size of output buffer.
	return buf;
};
