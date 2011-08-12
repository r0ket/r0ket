/* CCC MSN 0.1b @ CCC 2011 Germany
 * Developers:
 *  - Simpson
 *  - 0xCC
 * 
 * Base Codes:
 *  - sign.c (For Mesh Communication)
 *  - blink.c (For Led Notification)
 * 
 * */
#include <sysinit.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "funk/nrf24l01p.h"
#include "funk/rftransfer.h"

#include "lcd/render.h"
#include "lcd/print.h"

#include "usetable.h"

/*Global Communication Config*/
uint8_t mac[5] = {1,2,3,5,5};
struct NRF_CFG config = 
{
   .channel= 81,
   .txmac= "\x1\x2\x3\x5\x5",
   .nrmacs=1,
   .mac0=  "\x1\x2\x3\x5\x5",
   .maclen ="\x20",
};

/*Possible Messages*/
char msgs[5][20]={" Hi There!"," At Kourou"," At Baikonur","Party Hard!","Like a Boss!"};

void initDisplay(void);
void initMesh(void);
void waitMsg(void);
void blink(void);

/*Main Loop*/
void ram(void) 
{
	initDisplay();
	uint8_t key;
	do 
	{
		key = getInput();
		if(key == BTN_DOWN)
			waitMsg();
		else if (key == BTN_UP)
			initMesh();
    } while(key != BTN_ENTER);
}

/*Message Receiver*/
void waitMsg()
{
	uint8_t key;
	unsigned char buf[32];
	char sender_nick[10];
	int n,i;
	int index=0;
	int rcv =0;
	int try=0;
	do
	{
		key = getInput();
		if(1)
		{
			nrf_config_set(&config);
			n = nrf_rcv_pkt_time(100,32,buf);
			getInputWaitTimeout(100);
			if(n == 32)
			{
				index=buf[0];
				for(i=0;i<10;i++)
					sender_nick[i]=buf[i+1];
				lcdClear();
				lcdPrintln("");
				lcdPrintln("CCC MSN 0.1b");
				lcdPrintln("---------------");
				lcdPrintln(msgs[index]);        
				lcdPrintln("");        
				lcdPrintln("Received");
				lcdPrintln("");
				lcdPrintln("_______________");
				lcdPrintln(sender_nick);
				lcdRefresh();
				blink();
				rcv=1;
				try=0;
			} else if(rcv)
			{
				lcdClear();
				lcdPrintln("");
				lcdPrintln("CCC MSN 0.1b");
				lcdPrintln("-----------------");
				lcdPrintln("Waiting...");                
				lcdPrint("Try ");
				lcdPrintln(IntToStr(try,5,0));
				lcdPrintln("");
				lcdPrintln(msgs[index]);
				lcdPrintln("_______________");
				lcdPrintln(sender_nick);
				lcdRefresh();
			}
			try++;
		}
		if(!rcv)
		{
			lcdClear();
			lcdPrintln("");
			lcdPrintln("CCC MSN 0.1b");
			lcdPrintln("-----------------");
			lcdPrintln("Waiting...");        
			lcdPrintln("");        
			lcdPrint("Try ");
			lcdPrintln(IntToStr(try,5,0));
			lcdPrintln("");
			lcdPrintln("_______________");
			lcdPrintln("W8 Users Auto");
			lcdRefresh();
		}
	}while(key != BTN_ENTER);
	initDisplay();
}

/*Starter*/
void initDisplay(void)
{   
    lcdClear();
    lcdPrintln("");
    lcdPrintln("CCC MSN 0.1b");
    lcdNl();
    lcdPrintln("Commands:");        
    lcdPrintln("UP/Down");        
    lcdPrintln("Press to Exit!");
    lcdPrintln("");
    lcdPrintln("_______________");
    lcdPrintln(GLOBAL(nickname));
    lcdRefresh();
}

/*Start to Talk*/
void initMesh(void)
{
	char buf[32];
	uint8_t key;
	int index=0;
	int trys=0;
	int i;
	do 
	{
		key = getInput();
		if(key == BTN_RIGHT)
			index=(index+1)%5;
		if(key == BTN_UP)
		{
			memset(buf,0,32);
			buf[0]=index;
			for(i=0;i<11;i++)
				buf[i+1]=GLOBAL(nickname)[i];
			nrf_config_set(&config);
			nrf_snd_pkt_crc(32,(uint8_t *)buf);
			trys=trys+1;
		}
		lcdClear();
		lcdPrintln("");
		lcdPrintln("CCC MSN 0.1b");
		lcdPrintln("-----------------");
		lcdPrintln(msgs[index]);        
		lcdPrintln("");        
		lcdPrint("Try ");
		lcdPrintln(IntToStr(trys,5,0));
		lcdPrintln("");
		lcdPrintln("_______________");
		lcdPrintln("UP To Send Msg");
		lcdRefresh();
    } while(key != BTN_ENTER);
    initDisplay();
}

/*Led Blink*/
void blink(void)
{
	for (int x=0;x<20;x++)
	{
		gpioSetValue (RB_LED1, x%2); 
		delayms(50);
	}
}
