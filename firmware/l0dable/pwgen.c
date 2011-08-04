#include "basic/basic.h"

#include "usetable.h"

#define PW_LEN 8

void pw_cleanup(char * pw);
void pw_set(char * pw, uint16_t * k);

void ram(void)
{
	char pw[PW_LEN+1];
	uint16_t k[8];
	int button;
	memset(k, 0, 16);
	while(1){
		lcdClear();
		lcdNl();
		lcdPrintln(" password");
		lcdPrintln("    generator");
		lcdNl();
		lcdNl();
		pw_set(pw,k);
		pw_cleanup(pw);
		lcdPrint("   ");
		lcdPrintln(pw);
		lcdRefresh();
		delayms(23);
		while((button=getInputRaw())==BTN_NONE)
			delayms(23);
		if(button==BTN_LEFT) return;
	}
}

void pw_cleanup(char * pw)
{
	int i;
	for(i=0;i<PW_LEN;i++)
	{
		/* strip unwanted ascii chars */
		pw[i]&=0x7f;
		if(pw[i]<0x30)pw[i]+=0x30;
		if(pw[i]>0x7a)pw[i]-=0x10;
		if((pw[i]>'Z')&&(pw[i]<'a'))
			pw[i]-=0x10;
	}
}

void pw_set(char * pw, uint16_t * k)
{
	int i;
	memset(pw,0,PW_LEN); /* wipe old PW */
	for(i=0;i<4;i++)
		k[1]=getRandom();
	xxtea_encode_words(pw,PW_LEN/4,k);
	pw[PW_LEN]=0;
}

