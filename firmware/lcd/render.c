#include <string.h>

#include <sysdefs.h>
#include <render.h>
#include <decoder.h>
#include <fonts.h>
#include "basic/basic.h"
#include "fonts/smallfonts.h"

#include "filesystem/ff.h"

/* Global Variables */
const struct FONT_DEF * font = NULL;

struct EXTFONT efont;

FIL file; /* current font file */

/* Exported Functions */

void setIntFont(const struct FONT_DEF * font){
    memcpy(&efont.def,font,sizeof(struct FONT_DEF));
    efont.type=FONT_INTERNAL;
    font=NULL;
};

void setExtFont(const char *fname){
    if(strlen(fname)>8+4)
        return;
    strcpy(efont.name,fname);
//    memcpy(efont.name+strlen(fname),".f0n",5);

    efont.type=FONT_EXTERNAL;
    font=NULL;
};


int _getFontData(int type, int offset){
    UINT readbytes;
    UINT res;
    static uint16_t extras;
    static uint16_t character;
    static const void * ptr;

    if(efont.type == FONT_EXTERNAL){

    if (type == START_FONT){
        res = f_read(&file, &efont.def.u8Width, sizeof(uint8_t), &readbytes);
        res = f_read(&file, &efont.def.u8Height, sizeof(uint8_t), &readbytes);
        res = f_read(&file, &efont.def.u8FirstChar, sizeof(uint8_t), &readbytes);
        res = f_read(&file, &efont.def.u8LastChar, sizeof(uint8_t), &readbytes);
        res = f_read(&file, &extras, sizeof(uint16_t), &readbytes);
        return 0;
    };
    if (type == SEEK_EXTRAS){
        f_lseek(&file,6);
        return 0;
    };
    if(type == GET_EXTRAS){
        uint16_t word;
        res = f_read(&file, &word, sizeof(uint16_t), &readbytes);
        return word;
    };
    if (type == SEEK_WIDTH){
        f_lseek(&file,6+(extras*sizeof(uint16_t)));
        return 0;
    };
    if(type == GET_WIDTH || type == GET_DATA){
        uint8_t width;
        res = f_read(&file, &width, sizeof(uint8_t), &readbytes);
        return width;
    };
    if(type == SEEK_DATA){
        character=offset;
        f_lseek(&file,6+
                (extras*sizeof(uint16_t))+
                ((extras+font->u8LastChar-font->u8FirstChar)*sizeof(uint8_t))+
                (offset*sizeof(uint8_t))
                );
        return 0;
    };
    if(type == PEEK_DATA){
        uint8_t width;
        res = f_read(&file, &width, sizeof(uint8_t), &readbytes);
        f_lseek(&file,6+
                (extras*sizeof(uint16_t))+
                ((extras+font->u8LastChar-font->u8FirstChar)*sizeof(uint8_t))+
                (character*sizeof(uint8_t))
                );
        return width;
    };
#ifdef NOTYET
    }else{ // efont.type==FONT_INTERNAL

        if (type == START_FONT){
            memcpy(&efont.def,font,sizeof(struct FONT_DEF));
            return 0;
        };
        if (type == SEEK_EXTRAS){
            ptr=efont.def.charExtra;
            return 0;
        };
        if(type == GET_EXTRAS){
            uint16_t word;
            word=*(uint16_t*)ptr;
            ptr=((uint16_t*)ptr)+1;
            return word;
        };
        if (type == SEEK_WIDTH){
            ptr=efont.def.charInfo;
            return 0;
        };
        if(type == GET_WIDTH || type == GET_DATA){
            uint8_t width;
            width=*(uint8_t*)ptr;
            ptr=((uint8_t*)ptr)+1;
            return width;
        };
        if(type == SEEK_DATA){
            if(offset==REPEAT_LAST_CHARACTER)
                offset=character;
            else
                character=offset;

            ptr=efont.def.au8FontTable;
            return 0;
        };
#endif
    };

    /* NOTREACHED */
    return 0;
};

int _getIndex(int c){
#define ERRCHR (font->u8FirstChar+1)
    /* Does this font provide this character? */
    if(c<font->u8FirstChar)
        c=ERRCHR;
    if(c>font->u8LastChar && efont.type!=FONT_EXTERNAL && font->charExtra == NULL)
        c=ERRCHR;

    if(c>font->u8LastChar && (efont.type==FONT_EXTERNAL || font->charExtra != NULL)){
        if(efont.type==FONT_EXTERNAL){
            _getFontData(SEEK_EXTRAS,0);
            int cc=0;
            int cache;
            while( (cache=_getFontData(GET_EXTRAS,0)) < c)
                cc++;
            if( cache > c)
                c=ERRCHR;
            else
                c=font->u8LastChar+cc+1;
        }else{
            int cc=0;
            while( font->charExtra[cc] < c)
                cc++;
            if(font->charExtra[cc] > c)
                c=ERRCHR;
            else
                c=font->u8LastChar+cc+1;
        };
    };
    c-=font->u8FirstChar;
    return c;
};

uint8_t charBuf[MAXCHR];

int DoChar(int sx, int sy, int c){

//    font=NULL;
    if(font==NULL){
        if(efont.type==FONT_INTERNAL){
            font=&efont.def;
        }else if (efont.type==FONT_EXTERNAL){
            UINT res;
            res=f_open(&file, efont.name, FA_OPEN_EXISTING|FA_READ);
            if(res){
                efont.type=0;
                font=&Font_7x8;
            }else{
                _getFontData(START_FONT,0);
                font=&efont.def;
            };
        }else{
            font=&Font_7x8;
        };
    };

	/* how many bytes is it high? */
	char height=(font->u8Height-1)/8+1;
    char hoff=(8-(font->u8Height%8))%8;

	const uint8_t * data;
    int width,preblank=0,postblank=0; 
    do { /* Get Character data */
        /* Get intex into character list */
        c=_getIndex(c);

        /* starting offset into character source data */
        int toff=0;

        if(font->u8Width==0){
            if(efont.type == FONT_EXTERNAL){
                _getFontData(SEEK_WIDTH,0);
                for(int y=0;y<c;y++)
                    toff+=_getFontData(GET_WIDTH,0);
                width=_getFontData(GET_WIDTH,0);

                _getFontData(SEEK_DATA,toff);
                UINT res;
                UINT readbytes;
                res = f_read(&file, charBuf, width*height, &readbytes);
                if(res != FR_OK || readbytes<width*height)
                    return sx;
                data=charBuf;
            }else{
                for(int y=0;y<c;y++)
                    toff+=font->charInfo[y].widthBits;
                width=font->charInfo[c].widthBits;

                toff*=height;
                data=&font->au8FontTable[toff];
            };
            postblank=1;
        }else if(font->u8Width==1){ // NEW CODE
            if(efont.type == FONT_EXTERNAL){
                _getFontData(SEEK_WIDTH,0);
                for(int y=0;y<c;y++)
                    toff+=_getFontData(GET_WIDTH,0);
                width=_getFontData(GET_WIDTH,0);
                _getFontData(SEEK_DATA,toff);
                UINT res;
                UINT readbytes;
                uint8_t testbyte;
                res = f_read(&file, &testbyte, sizeof(uint8_t), &readbytes);
                if(testbyte>>4 ==15){
                    res = f_read(&file, &preblank, sizeof(uint8_t), &readbytes);
                    res = f_read(&file, &postblank, sizeof(uint8_t), &readbytes);
                    width-=3;
                    width/=height;
                    res = f_read(&file, charBuf, width*height, &readbytes);
                    if(res != FR_OK || readbytes<width*height)
                        return sx;
                    data=charBuf;
                }else{
                    _getFontData(SEEK_DATA,toff);
                    data=pk_decode(NULL,&width); // Hackety-hack
                };
            }else{
                // Find offset and length for our character
                for(int y=0;y<c;y++)
                    toff+=font->charInfo[y].widthBits;
                width=font->charInfo[c].widthBits;
                if(font->au8FontTable[toff]>>4 == 15){ // It's a raw character!
                    preblank = font->au8FontTable[toff+1];
                    postblank= font->au8FontTable[toff+2];
                    data=&font->au8FontTable[toff+3];
                    width=(width-3/height);
                }else{
                    data=pk_decode(&font->au8FontTable[toff],&width);
                }
            };

        }else{
            toff=(c)*font->u8Width*height;
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
