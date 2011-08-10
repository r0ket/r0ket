#include "filesystem/ff.h"
#include <basic/basic.h>
#include "usetable.h"

#define MAXSIZE 1024

//shows vcards
void ram(void)
{
	char filename[13];
	uint8_t buf[MAXSIZE];
	FRESULT res;
	UINT readbytes;
	FIL file;
	uint8_t key;
	int i,j=0,lines=0;
	char *beginline=buf;
	char help;

	selectFile(filename,"CRD");
	lcdClear();
	res=f_open(&file, (const char*)filename, FA_OPEN_EXISTING|FA_READ);
	res=f_read(&file, (char *)buf, MAXSIZE, &readbytes);
	for(i=0;i<readbytes;i++)
	{
		if(lines==7)
		{
			lines=0;
			lcdRefresh();
			while(1)
			{
			    	key = getInput();
				if( key == BTN_DOWN){
			            	break;
				}

			}
		}
		if(buf[i]=='\n')
		{
			j=0;
			buf[i]=0;
			lcdPrintln(beginline);
			beginline=&buf[i+1];
			lines++;
		}
		else if(j>13)
		{
			help=buf[i];
			j=0;
			buf[i]=0;
			lcdPrintln(beginline);
			beginline=&buf[i];
			buf[i]=help;
			lines++;
		}
		j++;
		
	}
	lcdRefresh();
	while (1)
	{
	    	key = getInput();
		if( key == BTN_LEFT ){
	            	break;
		}
	}

}

