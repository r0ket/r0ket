#include <sysdefs.h>
#include <render.h>
#include <fonts.h>

/* Global Variables */
const struct FONT_DEF * font = NULL;
char font_direction = FONT_DIR_LTR;

/* Exported Functions */
int DoChar(int sx, int sy, char c){
	int rx=RESX-sx-1;
	int ry=RESY-sy-font->u8Height;
	
	if(c<font->u8FirstChar || c>font->u8LastChar)
		c=font->u8FirstChar+1; // error

	int off;
	int x;
	int byte;
	int dmul=0;
	if(font_direction==FONT_DIR_RTL)
		dmul=1;
	else if (font_direction==FONT_DIR_LTR)
		dmul=-1;

	off=(c-font->u8FirstChar)*font->u8Width;
	char yidx=ry/8;
	if(0 && sy%8==0){
		for(x=0;x<font->u8Width;x++){
			buffer[(sx+x)+yidx*96]=font->au8FontTable[off+x];
		};
	}else{
		char yoff=ry%8;
		unsigned char mask;
		if(font->u8Height == 6)
			mask=255-3;
		else
			mask=255;

		if(font_direction==FONT_DIR_LTR)
			flip(mask);

		for(x=0;x<font->u8Width;x++){
			byte=font->au8FontTable[off+x];
			if(font_direction==FONT_DIR_LTR)
				flip(byte);
#ifdef SANITY
if(rx+x <0 || rx+x > RESX)
	gpioSetValue (CFG_LED_PORT, CFG_LED_PIN, CFG_LED_ON); 
if(yidx<0 || yidx> RESY/8) // Gah. non-integer height. XXX fixme
	gpioSetValue (CFG_LED_PORT, CFG_LED_PIN, CFG_LED_ON); 
#endif
			buffer[(rx+dmul*x)+yidx*96]&=~((mask)<<yoff);
			buffer[(rx+dmul*x)+yidx*96]|=byte<<yoff;

			buffer[(rx+dmul*x)+(yidx+1)*96]&=~((mask)>>(8-yoff));
			buffer[(rx+dmul*x)+(yidx+1)*96]|=byte>>(8-yoff);
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
		

