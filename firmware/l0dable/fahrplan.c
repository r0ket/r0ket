#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/print.h"
#include "lcd/render.h"
#include "lcd/display.h"
#include "filesystem/ff.h"
#include "filesystem/diskio.h"

#include "basic/config.h"

#include "usetable.h"

#define one ((uint32_t)1)

typedef uint8_t uchar;

static unsigned long iter=0;

static int getint3fromf(FIL *file);

void ram(void) {
    getInputWaitRelease();

    static char delay=15;
   
    char buf[20];

    int line;
    char startline=0;
    char *message=NULL;
    char filvers;
    char favers[5]; 
    unsigned char ob;
    int eventid;
    int evnext;
    int evprv;
    int evcur=5;


    lcdClear();
    /* setExtFont(GLOBAL(nickfont)); */
    setExtFont(NULL);

    /* nickheight=getFontHeight(); */

    lcdClear();

        FIL file;
        FRESULT res;
        UINT readbytes;
    
        res=f_open(&file, "FAHRPLAN.SCD", FA_OPEN_EXISTING|FA_READ|FA_WRITE);
        if(res) message="FOPEN ERROR";
        res=f_read(&file, &filvers, 1, &readbytes); 
        res+=f_read(&file, (char *)favers, 4, &readbytes); 
        favers[4]=0;


        lcdClear();
        DoString(0,0,"Fahrplan ");
        DoString(60,0,favers);
        DoString(0,16,"SW Rel. V0.001");
        if(filvers!=2) {
        DoString(0,24,"Incompatible  ");
        DoString(0,32,"Binary. Get   ");
        DoString(0,40,"update from   ");
        DoString(0,48,"r0ket.de      ");
        DoString(0,56,"Filefmt.: ");
        DoInt(60,56,filvers);
        lcdDisplay();
	getInputWait();
	getInputWaitRelease();
        return;
        } 
        DoString(0,24,"On C3 get new ");
        DoString(0,32,"version from  ");
        DoString(0,40,"r0ket.de for  ");
        DoString(0,48,"better UI     ");
        /* DoInt(0,24,filvers); */
        lcdDisplay();
        delayms(1000);
        DoString(0,56," PRESS BUTTON ");
        lcdDisplay();
	getInputWait();
	getInputWaitRelease();

        evcur=getint3fromf(&file);

    while (1) {
        ++iter;
        lcdDisplay();
        lcdClear();
        char title[15];
        char info[15];
        int evtext; 

#ifdef SIMULATOR
#endif
#ifndef SIMULATOR
        f_lseek(&file,evcur);

        eventid=getint3fromf(&file);
        evprv=getint3fromf(&file);
        evnext=getint3fromf(&file);

        res+=f_read(&file, (char *)title, 15, &readbytes); 
        res+=f_read(&file, (char *)info, 15, &readbytes); 
        evtext=f_tell(&file);

#endif
        /* for(line=0; line<4; line++) {
          DoString(0, 8*line+8,tdes[0][line+startline]);
        } */
#ifndef SIMULATOR
        title[14]=0;
        DoString(0,0,title);
        info[14]=0;
        DoString(0,8,info);

        int notend=1; 
        for(line=0; line<6 && notend; line++) {
        f_lseek(&file,evtext+(startline+line)*14);
        res+=f_read(&file, (char *)buf, 15, &readbytes); 
        if(res) message="READ ERROR"; else message="READ: 0";
        buf[14]=0;
        DoString(0,8*line+20,buf);
        if(!buf[0])notend=0;
        } 

/* Debug:
        DoInt(0,32,eventid);
        DoInt(0,40,evprv);
        DoInt(50,40,evnext);
        if(message) DoString(0, 8*5+8,message);
*/ 
#endif
        lcdDisplay();

	// char key=stepmode?getInputWait():getInputRaw();
	//
	char key=getInputWait();
	switch(key) {
        // Buttons: Right change speed, Up hold scrolling
	case BTN_ENTER:
	  getInputWaitRelease();
          f_lseek(&file,5);
          ob=evcur%256;
          res+=f_write(&file, (char *)&ob, 1, &readbytes);
          ob=evcur>>8;
          res+=f_write(&file, (char *)&ob, 1, &readbytes);
	  return;
	case BTN_RIGHT:
	  getInputWaitRelease();
          if(evnext)evcur=evnext;
          startline=0;
          break;
	case BTN_UP:
	  getInputWaitRelease();
          if(startline)startline--;
	  break;
	case BTN_LEFT:
	  getInputWaitRelease();
          if(evprv)evcur=evprv;
          startline=0;
          break;
	case BTN_DOWN:
	  getInputWaitRelease();
          if(notend)startline++;
	  break;
	}
        delayms_queue_plus(delay,0);
    }
    return;
}

static int getint3fromf(FIL *file) {
    unsigned char ob;
    UINT readbytes;
    int res;

    f_read(file, (char *)&ob, 1, &readbytes); 
    res=ob;
    f_read(file, (char *)&ob, 1, &readbytes); 
    res+=ob*256;
    f_read(file, (char *)&ob, 1, &readbytes); 
    res+=ob*256*256;

    return res;
}
