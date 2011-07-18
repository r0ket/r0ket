#include <sysinit.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "basic/basic.h"
#include "lcd/render.h"
#include "lcd/allfonts.h"

#include "ecc.c"
void backlightInit(void);

/**************************************************************************/

void main_schneider(void) {
    //int yctr=8;
    int dx=0;
    char key;
    int ctr = 1;
    backlightInit();
    font_direction = FONT_DIR_LTR; // LeftToRight is the default
    //yctr=18;
        bitstr_parse(poly, "800000000000000000000000000000000000000c9");
        bitstr_parse(coeff_b, "20a601907b8c953ca1481eb10512f78744a3205fd");
        bitstr_parse(base_x, "3f0eba16286a2d57ea0991168d4994637e8343e36");
        bitstr_parse(base_y, "0d51fbc6c71a0094fa2cdd545b11c5c0c797324f1");
        bitstr_parse(base_order, "40000000000000000000292fe77e70c12a4234c33");

        ECIES_generate_key_pair();          // generate a public/private key pair
    while (1) {
        key= getInput();
        font=&Font_7x8;

        // Easy flashing
        if(key==BTN_LEFT){
            DoString(0,8,"Enter ISP!");
            lcdDisplay();
            ISPandReset();
        };

        // Display nickname
        //font = &Font_Ubuntu36pt;
        dx=DoString(0,0,"Test");
        dx=DoInt(dx,0,ctr++);
        lcdDisplay();
        encryption_decryption_demo("This is encrypted",
                        "1c56d302cf642a8e1ba4b48cc4fbe2845ee32dce7", 
                        "45f46eb303edf2e62f74bd68368d979e265ee3c03",
                        "0e10e787036941e6c78daf8a0e8e1dbfac68e26d2");
    }
    return;
}

void tick_schneider(void){
    return;
};

