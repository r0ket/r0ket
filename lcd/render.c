#include <sysdefs.h>
#include <render.h>
#include <fonts.h>

/* Global Variables */
const struct FONT_DEF * font = NULL;
char font_direction = FONT_DIR_LTR;

/* Exported Functions */

int DoChar(int sx, int sy, char c){
	int x;
	int y;

	/* "real" coordinates. Our physical display is upside down */
	int rx=RESX-sx-1;
	int ry=RESY-sy-font->u8Height;

	/* how many bytes is it high? */
	char height=(font->u8Height-1)/8+1;

	/* Does this font provide this character? */
	if(c<font->u8FirstChar || c>font->u8LastChar)
		c=font->u8FirstChar+1; // error

	/* starting offset into character source data */
	int off,width; 
	if(font->u8Width==0){
		off=font->charInfo[c-font->u8FirstChar].offset;
		width=font->charInfo[c-font->u8FirstChar].widthBits;
//		width=(font->charInfo[c-font->u8FirstChar].offset-off)/8;
	}else{
		off=(c-font->u8FirstChar)*font->u8Width*height;
		width=font->u8Width;
	};

	/* raw character data */
	int byte;
	unsigned char mask;

	/* print forward or backward? */
	int dmul=0;
	if(font_direction==FONT_DIR_RTL)
		dmul=1;
	else if (font_direction==FONT_DIR_LTR)
		dmul=-1;


	/* break down the position on byte boundaries */
	char yidx=ry/8;
	char yoff=ry%8;

	/* multiple 8-bit-lines */
	for(y=0;y<=height;y++){
		mask=255; // XXX: Fixme
		if(y==0){
			mask=mask>>(yoff);
		} else if(y==height){
			if(font->u8Height==6)
				mask=252;
			if(font->u8Height==10)
				mask=192;
			if(font->u8Height==16)
				mask=255;
			mask=mask<<(8-yoff);
		};

		if(font_direction==FONT_DIR_LTR)
			flip(mask);

		for(x=0;x<font->u8Width;x++){
			unsigned char b1,b2;
			if(y==0)
				b1=0;
			else
				b1=font->au8FontTable[off+x*height+y-1];
			if(y==height)
				b2=0;
			else
				b2=font->au8FontTable[off+x*height+y];

			byte= (b1<<(8-yoff)) | (b2>>yoff);
			if(font_direction==FONT_DIR_LTR)
				flip(byte);

			buffer[(rx+dmul*x)+(yidx+y)*RESX]&=~mask;
			buffer[(rx+dmul*x)+(yidx+y)*RESX]|=byte;
		};
	};
	return sx-dmul*x;
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
		

