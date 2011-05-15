// Demo Compressed font decoder. 
// Just used for debugging
// nothing to see, move along...
// (c) by Sec 5/2011, BSD licence

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define uint8_t unsigned char
#define uint16_t unsigned short

#include "ubuntu17.c"

const struct FONT_DEF * font = &Font_Ubuntu17pt;

#define MAXCHR (24*3)
static uint8_t buf[MAXCHR];

int decode(char c){
	int off=0;				// Offset into au8FontTable for bytestream
	int length;				// Length of character bytestream
	int height;				// Height of character in bytes
	int hoff;
	uint8_t * bufptr=buf;	// Output buffer for decoded character

	const FONT_CHAR_INFO* fci=font->charInfo;

	// Find offset and length for our character
	off=0;
	for(int y=0;y<c-font->u8FirstChar;y++)
		off+=fci[y].widthBits;
	length=fci[c-font->u8FirstChar].widthBits;

	height=(font->u8Height-1)/8+1;
	hoff=font->u8Height%8;

	printf("Doing %c: off=%d, len=%d\n",c,off,length);

	// Local function: Get next nibble.
	int ctr=0;  // offset for next nibble
	char gnn(){ // Get next nibble
		static int byte;
		static int hilo=0; // 0= high nibble next, 1=low nibble next
		int val;
		hilo=1-hilo;
		if(hilo==1){
			byte=font->au8FontTable[off+ctr];
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



	if(font->au8FontTable[off+0]>>4 == 15){
		// This is a raw character!
		int preblank,postblank;
		const uint8_t *rawptr;
		preblank=  font->au8FontTable[off+1];
		postblank= font->au8FontTable[off+2];
		rawptr=&font->au8FontTable[off+2];
		printf("Raw character: pre=%d, post=%d\n",preblank,postblank);
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
		printf("\nCtr: %d, byte: %x, Process: %d\n",ctr, font->au8FontTable[off+ctr], nyb);

		if(nyb==15){
			repeat++;
			printf("repeat this line \n");
			continue;
		}else if(nyb==14){
			nyb=upl(0);
			nyb+=1;
			printf("14-decode: %d\n",nyb);
			repeat+=nyb;
			continue;
		}else if(nyb>DNY){
			nyb=(16*(nyb-DNY-1))+gnn()+DNY+1;
		}else if(nyb==0){
			nyb=upl(1);
			nyb+=(16*(13-DNY)+DNY)-16;
			printf("0-decode: %d\n",nyb);
		};

		/* Generate & output bits */

		printf("have %d bits. Got %d (%d)-bits...(r=%d)",pos, nyb, curbit,repeat);
		while(nyb-->0){
			if(pos==0){
				*bufptr=0;
			};
			if(curbit==1){
				*bufptr|=1<<(7-pos);
			};
			pos++;
			if( hoff>0 && ((bufptr-buf)%height)==(height-1) && (pos==hoff)){
				// Incomplete last line.
				printf("Incomplete last byte output: %d:",(bufptr-buf)%height);
				pos=8;
			};
			if(pos==8 ){
				printf("BYTE: 0x%02x\n",*bufptr);
				bufptr++;
				if((bufptr-buf)%height==0){ // End of column?
					while(repeat>0){
						for(int y=0;y<height;y++){
							bufptr[0]=bufptr[-3];
							bufptr++;
						};
						printf("repeated last line\n");
						repeat--;
					};
				};
				pos=0;
				printf("remaining bits: %d\n",nyb);
			};
		};
		curbit=1-curbit;
	};
		
	printf("\n");

	return bufptr-buf;
};

int main(void){

	int x,sz;

	sz=decode('H');
	for(x=0;x<sz;x++){
		printf(" 0x%02x,",buf[x]);
		if(x%3==2){
			printf("\n");
		};
	};

	exit(0);
}
