#include <sysdefs.h>
#include <render.h>
#include <fonts.h>

/* Global Variables */
const struct FONT_DEF * font = NULL;
char font_direction = FONT_DIR_LTR;

/* Exported Functions */

uint8_t * pk_decode(const uint8_t * data,int*len);
int DoChar(int sx, int sy, char c){
	int x=0;
	int y;

	/* how many bytes is it high? */
	char height=(font->u8Height-1)/8+1;

	const uint8_t * data;

	/* "real" coordinates. Our physical display is upside down */
	int rx=RESX-sx-1;
	int ry=RESY-sy-font->u8Height;

	/* Does this font provide this character? */
	if(c<font->u8FirstChar || c>font->u8LastChar)
		c=font->u8FirstChar+1; // error

	/* starting offset into character source data */
	int toff,width,preblank,blank; 
	if(font->u8Width==0){
		toff=0;
		width=font->charInfo[c-font->u8FirstChar].widthBits;
		for(y=0;y<c-font->u8FirstChar;y++)
			toff+=font->charInfo[y].widthBits;
		toff*=height;
		data=&font->au8FontTable[toff];
		preblank=0;
		blank=1;
/*	}else if(font->u8Width==1){
		FONT_CHAR_INFO_v2 * fci=(FONT_CHAR_INFO_v2*)font->charInfo;
		off=0;
		width=fci[c-font->u8FirstChar].widthBits;
		for(y=0;y<c-font->u8FirstChar;y++)
			off+=fci[y].widthBits;
		off*=height;
		preblank=fci[y].preblank;
		blank=fci[y].blank; */
	}else if(font->u8Width==1){ // NEW CODE
		// Find offset and length for our character
		toff=0;
		for(int y=0;y<c-font->u8FirstChar;y++)
			toff+=font->charInfo[y].widthBits;
		width=font->charInfo[c-font->u8FirstChar].widthBits;

		data=pk_decode(&font->au8FontTable[toff],&width);
		preblank=0;
		blank=0;
	}else{
		toff=(c-font->u8FirstChar)*font->u8Width*height;
		width=font->u8Width;
		data=&font->au8FontTable[toff];
		preblank=0;
		blank=0;
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
	for(y=0;y<=height;y++){
		int m=yoff+font->u8Height-8*y;
		if(m>8)m=8;
		if(m<0)m=0;
		mask=255<<(8-m);

		if(y==0){
			mask=mask>>yoff;
		};

		if(mask==0) // Optimize :-)
			break;
//		buffer[(rx-dmul)+(yidx+y)*RESX]=5;

		if(font_direction==FONT_DIR_LTR)
			flip(mask);

		for(m=1;m<=preblank;m++){
			buffer[(rx-dmul*(m))+(yidx+y)*RESX]&=~mask;
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

			buffer[(rx+dmul*x)+(yidx+y)*RESX]&=~mask;
			buffer[(rx+dmul*x)+(yidx+y)*RESX]|=byte;
		};
		for(m=0;m<blank;m++){
			buffer[(rx+dmul*(x+m))+(yidx+y)*RESX]&=~mask;
		};
	};
	return sx-dmul*(x+preblank+blank);
};

int DoString(int sx, int sy, char *s){
	char *c;
	for(c=s;*c!=0;c++){
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
		
	for (len=(mxlen-1);len>=0;len--){
		s[len]=(num%10)+'0';
		if(num==0){
			s[len]=' '; // configurable?
			o=s+len;
			break;
		};
		num/=10;
	};
	if(neg)
		*o='-';
	return DoString(sx,sy,o);
#undef mxlen
};

int DoIntX(int sx, int sy, unsigned int num){
#define mxlen 8
	char s[(mxlen+1)];
	char * o=s;
	int len;
	s[mxlen]=0;
	for (len=(mxlen-1);len>=0;len--){
		s[len]=(num%16)+'0';
		if(s[len]>'9')
			s[len]+='A'-'9'-1;
		if(num==0){
//			s[len]=' '; // configurable?
//			o=s+len; break;
		};
		num/=16;
	};
	return DoString(sx,sy,o);
#undef mxlen
};
		

