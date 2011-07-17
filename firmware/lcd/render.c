#include <sysdefs.h>
#include <render.h>
#include <decoder.h>
#include <fonts.h>

/* Global Variables */
const struct FONT_DEF * font = NULL;
char font_direction = FONT_DIR_LTR;

/* Exported Functions */

int DoChar(int sx, int sy, int c){

	/* how many bytes is it high? */
	char height=(font->u8Height-1)/8+1;
    char hoff=(8-(font->u8Height%8))%8;

	const uint8_t * data;
    int width,preblank=0,postblank=0; 
    do { /* Get Character data */
        /* Does this font provide this character? */
        if(c<font->u8FirstChar)
            c=font->u8FirstChar+1; // error
        if(c>font->u8LastChar && font->charExtra == NULL)
            c=font->u8FirstChar+1; // error

        if(c>font->u8LastChar && font->charExtra != NULL){
            int cc=0;
            while( font->charExtra[cc] < c)
                cc++;
            if(font->charExtra[cc] > c)
                c=font->u8FirstChar+1; // error
            else
                c=font->u8LastChar+cc+1;
        };

        /* starting offset into character source data */
        int toff=0;

        if(font->u8Width==0){
            for(int y=0;y<c-font->u8FirstChar;y++)
                toff+=font->charInfo[y].widthBits;
            width=font->charInfo[c-font->u8FirstChar].widthBits;

            toff*=height;
            data=&font->au8FontTable[toff];
            postblank=1;
        }else if(font->u8Width==1){ // NEW CODE
            // Find offset and length for our character
            for(int y=0;y<c-font->u8FirstChar;y++)
                toff+=font->charInfo[y].widthBits;
            width=font->charInfo[c-font->u8FirstChar].widthBits;

            if(font->au8FontTable[toff]>>4 == 15){ // It's a raw character!
                preblank = font->au8FontTable[toff+1];
                postblank= font->au8FontTable[toff+2];
                data=&font->au8FontTable[toff+3];
                width=(width-3/height);
            }else{
                data=pk_decode(&font->au8FontTable[toff],&width);
            }
        }else{
            toff=(c-font->u8FirstChar)*font->u8Width*height;
            width=font->u8Width;
            data=&font->au8FontTable[toff];
        };

    }while(0);

	/* "real" coordinates. Our physical display is upside down */
#define xy_(x,yb) ( ( (RESY_B-1) -(yb)) * RESX + \
                    ( (RESX-1)   -( x)) )

	int x=0;

	/* raw character data */
	int byte;
	unsigned char mask;

    /* Our display height is non-integer. Adjust for that. */
    sy+=RESY%8;

    /* Fonts may not be byte-aligned, shift up so the top matches */
    sy-=hoff;

	/* break down the position on byte boundaries */
	signed int yidx=sy/8;
	signed int yoff=sy%8;

    if(yoff<0) { /* % operator is stupid on negative values */
        yoff+=8;
        yidx-=1;
    };

    sx+=preblank;

	/* multiple 8-bit-lines */
	for(int y=0;y<=height;y++){
        if(yidx+y<0)
            continue;
        if(yidx+y>=RESY_B)
            continue;

        // Number of bits we need to do in this line:
		int m=yoff+font->u8Height-8*y;
		m=yoff+height*8-y*8;
		if(m>8)m=8;
		if(m<0)m=0;
		mask=255>>(8-m); // Generate bitmask

        /* Empty space at top is not to be masked, we consider it
           not to be part of the font */
		if(y==0){
			mask=mask<<(yoff+hoff);
		}else if(y==1 && yoff+hoff>8){
			mask=mask<<(yoff+hoff-8);
        };

		if(mask==0) // Optimize :-)
			continue;

        flip(mask);

        /* Optional: empty space to the left */
		for(int b=1;b<=preblank;b++){
            if(sx-b<0)
                continue;
            if(sx-b>=RESX)
                continue;
			lcdBuffer[xy_(sx-b,yidx+y)]&=~mask;
		};
        /* Render character */
		for(x=0;x<width;x++){
			unsigned char b1,b2;
			if(y==0)
				b1=0;
			else
				b1=data[x*height+(height-1)-(y-1)];
			if(y==height)
				b2=0;
			else
				b2=data[x*height+(height-1)-(y)];

			byte= (b1>>(8-yoff)) | (b2<<yoff);
            flip(byte);

            if(sx+x<0)
                continue;
            if(sx+x>=RESX)
                continue;
			lcdBuffer[xy_(sx+x,yidx+y)]&=~mask;
			lcdBuffer[xy_(sx+x,yidx+y)]|=byte;
		};
        /* Optional: empty space to the right */
		for(int m=0;m<postblank;m++){
            if(sx+x+m<0)
                continue;
            if(sx+x+m>=RESX)
                continue;
			lcdBuffer[xy_(sx+x+m,yidx+y)]&=~mask;
		};
	};
	return sx+(width+postblank);
};

#define UTF8
// decode 2 and 4-byte utf-8 strings.
#define UT2(a)   ( ((a[0]&31)<<6)  + (a[1]&63) )
#define UT3(a)   ( ((a[0]&15)<<12) + ((a[1]&63)<<6) + (a[2]&63) )

int DoString(int sx, int sy, const char *s){
	const char *c;
	int uc;
	for(c=s;*c!=0;c++){
#ifdef UTF8
		/* will b0rk on non-utf8 */
		if((*c&(128+64+32))==(128+64) && c[1]!=0){
			uc=UT2(c); c+=1;
			sx=DoChar(sx,sy,uc);
		}else if( (*c&(128+64+32+16))==(128+64+32) && c[1]!=0 && c[2] !=0){
			uc=UT3(c); c+=2;
			sx=DoChar(sx,sy,uc);
		}else
#endif
		sx=DoChar(sx,sy,*c);
	};
	return sx;
};

int DoInt(int sx, int sy, int num){
#define mxlen 5
	char s[(mxlen+1)];
	char * o=s;
	int len;
	s[mxlen]=0;
	char neg=0;

	if(num<0){
		num=-num;
		neg=1;
	};
    if(num==0){
        neg=2;
    };
		
	for (len=(mxlen-1);len>=0;len--){
		s[len]=(num%10)+'0';
		if(num==0){
			s[len]=' '; // configurable?
			o=s+len;
			break;
		};
		num/=10;
	};
	if(neg==1)
		*o='-';
    if(neg==2)
        *o='0';
    if(neg==0)
        o++;
	return DoString(sx,sy,o);
#undef mxlen
};

int DoIntXn(int sx, int sy, unsigned int num, unsigned int mxlen){
	char s[(mxlen+1)];
	char * o=s;
	int len;
	s[mxlen]=0;
	for (len=(mxlen-1);len>=0;len--){
		s[len]=(num%16)+'0';
		if(s[len]>'9')
			s[len]+='A'-'9'-1;
		num/=16;
	};
	return DoString(sx,sy,o);
};

int DoIntX(int sx, int sy, unsigned int num){
    return DoIntXn(sx, sy, num, 8);
};

int DoCharX(int sx, int sy, unsigned char num){
    return DoIntXn(sx, sy, num, 2);
};

int DoShortX(int sx, int sy, uint16_t num){
    return DoIntXn(sx, sy, num, 4);
};
