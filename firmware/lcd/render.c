#include <sysdefs.h>
#include <render.h>
#include <decoder.h>
#include <fonts.h>

/* Global Variables */
const struct FONT_DEF * font = NULL;
char font_direction = FONT_DIR_LTR;

/* Exported Functions */

int DoChar(int sx, int sy, int c){
	int x=0;

	/* how many bytes is it high? */
	char height=(font->u8Height-1)/8+1;

	const uint8_t * data;

	/* "real" coordinates. Our physical display is upside down */
	int rx=RESX-sx-1;
	int ry=RESY-sy-font->u8Height;

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
	int toff=0,width,preblank=0,blank=0; 

	if(font->u8Width==0){
		for(int y=0;y<c-font->u8FirstChar;y++)
			toff+=font->charInfo[y].widthBits;
		width=font->charInfo[c-font->u8FirstChar].widthBits;

		toff*=height;
		data=&font->au8FontTable[toff];
		blank=1;
	}else if(font->u8Width==1){ // NEW CODE
		// Find offset and length for our character
		for(int y=0;y<c-font->u8FirstChar;y++)
			toff+=font->charInfo[y].widthBits;
		width=font->charInfo[c-font->u8FirstChar].widthBits;

		if(font->au8FontTable[toff]>>4 == 15){ // It's a raw character!
			preblank = font->au8FontTable[toff+1];
			blank= font->au8FontTable[toff+2];
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

	// boundary sanity checks
	if(sx<0 || sy<0 || sx >= RESX || (sy+font->u8Height) >= RESY)
		return sx; // nothing printed.

	/* raw character data */
	int byte;
	unsigned char mask;

	/* print forward or backward? */
	int dmul=0;
	if(font_direction==FONT_DIR_RTL){
		dmul=1;
		if(sx-(width+preblank+blank)<=0) // sanity check for left side
			return sx;
	} else if (font_direction==FONT_DIR_LTR){
		dmul=-1;
		if(sx+(width+preblank+blank)>=RESX) // sanity check for right side
			return sx;
	};

	/* break down the position on byte boundaries */
	char yidx=ry/8;
	char yoff=ry%8;

	rx+=dmul*preblank;

	/* multiple 8-bit-lines */
	for(int y=0;y<=height;y++){
		int m=yoff+font->u8Height-8*y;
		if(m>8)m=8;
		if(m<0)m=0;
		mask=255<<(8-m);

		if(y==0){
			mask=mask>>yoff;
		};

		if(mask==0) // Optimize :-)
			break;

		if(font_direction==FONT_DIR_LTR)
			flip(mask);

		for(int m=1;m<=preblank;m++){
			lcdBuffer[(rx-dmul*(m))+(yidx+y)*RESX]&=~mask;
		};
		for(x=0;x<width;x++){
			unsigned char b1,b2;
			if(y==0)
				b1=0;
			else
				b1=data[x*height+y-1];
			if(y==height)
				b2=0;
			else
				b2=data[x*height+y];

			byte= (b1<<(8-yoff)) | (b2>>yoff);
			if(font_direction==FONT_DIR_LTR)
				flip(byte);

			lcdBuffer[(rx+dmul*x)+(yidx+y)*RESX]&=~mask;
			lcdBuffer[(rx+dmul*x)+(yidx+y)*RESX]|=byte;
		};
		for(int m=0;m<blank;m++){
			lcdBuffer[(rx+dmul*(x+m))+(yidx+y)*RESX]&=~mask;
		};
	};
	return sx-dmul*(x+preblank+blank);
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
