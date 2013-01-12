#include <sysinit.h>

#include "basic/basic.h"

//#include "lcd/render.h"
//#include "lcd/display.h"
//#include "lcd/allfonts.h"
//#include "lcd/backlight.h"
//#include "lcd/print.h"

#include "usetable.h"

//void backlightInit(void);
void incBacklight(void);
void decBacklight(void);
void paintLogo(bool);
void leiwandDisplay(void);

/**************************************************************************/
#include "leiwand.h"
/**************************************************************************/

void ram(void) {
    int key;
    while (1) {
        leiwandDisplay();
        // Exit on enter+left
        key=getInputRaw();
        if(key== BTN_ENTER + BTN_LEFT)
            return;
    }
    return;
}

void leiwandInit(void) {
    paintLogo(false);
    delayms(200);
    DoString(0,0,"Boot...");
    lcdDisplay();
    delayms(1000);
}

void leiwandDisplay(void) {
    int dx=0;
    char key = 0;
    //backlightInit();
    int i;
    int j;
    char str[] = "LeiwandVille";
    char msg[] = " Just cool! ";
#define msglen 12 
    int pos[msglen];
    int dir[msglen];
    int round = 0;
    bool upPressed=false;
    bool rightPressed=false;
    bool downPressed=false;
    int demoround = 0;

    //lcdToggleFlag(LCD_INVERTED);

    for(i = 0; i < msglen; i++) {
        pos[i] = 1 + (i * 2);
        dir[i] = 0;
    }
   
    while (1) {
        lcdDisplay();
        delayms(20);

        key= getInputRaw();

        // Easy flashing
        if((key&(BTN_ENTER|BTN_LEFT))==(BTN_ENTER|BTN_LEFT)){
            //DoString(0,8,"Enter ISP!");
            //lcdDisplay();
            //ISPandReset();
            return;
        }

        /*
        // INVERT
        if(((key&(BTN_RIGHT))==(BTN_RIGHT))){
            if(!rightPressed) {
                lcdToggleFlag(LCD_INVERTED);
            }
            rightPressed = true;
        } else {
            rightPressed = false;
        }
        */

        // BRIGHTER
        if(((key&(BTN_UP))==(BTN_UP))){
            if(!upPressed) {
                incBacklight();
            }
            upPressed = true;
        } else {
            upPressed = false;
        }
        
        // DARKER
        if(((key&(BTN_DOWN))==(BTN_DOWN))){
            if(!downPressed) {
                decBacklight();
            }
            downPressed = true;
        } else {
            downPressed = false;
        }


        round++;
        if(round > 200) {
            round = 0;
            if(demoround == 1) {
                paintLogo(true);
            } else {
                paintLogo(false);
                delayms(1000);
            }
            demoround++;
        }
        if(demoround == 4) {
            demoround = 0;
        }

        for(i = 0; i < msglen; i++) {
            if(dir[i] == 1) {
                if(pos[i] >= 30) {
                    dir[i] = 0;
                } else {
                    pos[i]++;
                }
            } else {
                if(pos[i] == 0) {
                    dir[i] = 1;
                } else {
                    pos[i]--;
                }
            }
        }

        lcdFill(0);
        //font=&Font_7x8;
        for(i = 0; i < msglen; i++) {
            if(str[i] != 32) {
                dx=DoChar(i*8, pos[i], str[i]);
            }
        }
        if(round < 100) {
            // Second line of message
            for(i = 0; i < msglen; i++) {
                if(msg[i] != 32) {
                    dx=DoChar(i*8, 15+pos[i], msg[i]);
                }
            }
        } else {
            // INVADERS
            //font = &Font_Invaders;
            for(i = 2; i < 10; i+= 2) {
                dx=DoChar(i*8, 15+pos[i], 'x');
            }
        }
        //font=&Font_7x8;
        if(round < 50) {
            dx=DoString(0, 60, "  Visit us!  ");
        } else if(round < 100) {
            dx=DoString(0, 60, "   We are    ");
        } else if(round < 150) {
            dx=DoString(0, 60, " next to the ");
        } else {
            dx=DoString(0, 60, " phone booth ");
            if((round % 6) < 3) {
                for(i = 0; i < 96; i++) {
                    for(j = 0; j < 8; j++) {
                        lcdSetPixel(i, j+60, !lcdGetPixel(i, j+60));
                    }
                }
            }
        }
        /*
        if(key&BTN_ENTER){
            lcdPrintInt(ctr++);
            lcdPrintln(".");
            while(getInputRaw())delayms(10);
        };
		if(key&BTN_RIGHT){
			lcdShift(1,0,wrap);
		}
		if(key&BTN_LEFT){
			lcdShift(-1,0,wrap);
		}
		if(key&BTN_UP){
			lcdShift(0,1,wrap);
		}
		if(key&BTN_DOWN){
			lcdShift(0,-1,wrap);
		}
        */

        //font = &Font_Ubuntu36pt;
    }
    return;
}

void tick_scroll(void){
    return;
}

void incBacklight(void) {
    /*
    uint32_t brightness = backlightGetBrightness();
    if (brightness < 100) {
        backlightSetBrightness(brightness + 10);
    }
    */
    return;
}

void decBacklight(void) {
    /*
    uint32_t brightness = backlightGetBrightness();
    if (brightness > 0) {
        backlightSetBrightness(brightness - 10);
    }
    */
    return;
}

void paintLogo(bool animate) {
    int i;
    int j;
    int t;
    uint8_t pixel;
    int bc = 8;
    int next = 0;
    int mask = 1;


    lcdFill(0);
    lcdDisplay();
    for(i = 0; i < 68; i++) {
        for(j = 0; j < 68; j++) {
            if(bc == 8) {
                mask = 1;
                bc = 0;
                pixel = lv_logo[next];
                next++;
            }
            if((pixel & mask) > 0) {
                lcdSetPixel(14+j, i, true);
            } else {
                lcdSetPixel(14+j, i, false);
            }
            bc++;
            mask = mask << 1;
            pixel >> 2;
        }
        //lcdDisplay();
    }
    lcdDisplay();
    if(!animate) {
        return;
    }

    /*
    for(i = 0; i < 30; i++) {
        delayms(20);
    }
    */

    for(t=0; t < 8; t++) {
        for(i=0; i < 96; i++) {
            for(j=0; j < 68; j++) {
                lcdSetPixel(i, j, !lcdGetPixel(i,j));
            }
            lcdDisplay();
        }
    }

    /*
    for(i = 0; i < 50; i++) {
        delayms(20);
    }
    */

    return;
}
