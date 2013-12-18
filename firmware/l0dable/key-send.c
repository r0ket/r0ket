#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/render.h"
#include "lcd/print.h"

#include "funk/nrf24l01p.h"
#include "filesystem/ff.h"

#include "usetable.h"

/****************************************************************/
/* r0ket l0dable to send keys as an Microsoft keyboard          */
/* expects a KBD.MAC file containing lines with channel and mac */
/* expects a KBD.TXT file containing <stuff to send>            */
/*                                                              */
/* crsr right/left changes through channel/mac                  */
/* crsr enter      exits                                        */
/****************************************************************/

#define MAC_TABLE_SIZE 50

typedef uint16_t crc;

void setup_send(char*chmac);
signed char read_chmac(signed int lineno, char chmac[6]);
char send_key(char key,int ctr);

struct NRF_CFG config = {
    .channel= 1,
    .txmac= "\x0\x0\x0\x0\x0",
    .nrmacs=1,
    .mac0=  "\x0\x0\x0\x0\x0",
    .maclen ="\x14", /* how many bytes to receive */
};

void ram(void) {
    uint8_t key = BTN_NONE;
    int debug=1;
    int i;
    int myctr=0;
    int lineno=1;
    /*                             1               2               3      
                   0123456789abcdef0123456789abcdef0123456789abcdef01 2345678 */
    char * keys = "____abcdefghijklmnopqrstuvwxyz1234567890#_<_ -=[]\\_;'`,./";
    char chmac[6];

    FIL file;
    int res;

    lcdPrintln("enter ");
    read_chmac(lineno,chmac);
    lcdPrintln("read ");
    setup_send(chmac);
    lcdPrintln("setup ");

    do {
        signed char n;
        int k;
        char aabit;
        char chr[2];
        int pl;

        key=getInputRaw();
        if(key==BTN_UP){
            // empty.
            lcdRefresh();
            while(getInputRaw()!=BTN_NONE){delayms(10);};
        };
        if(key==BTN_DOWN){
            nrf_write_reg(R_STATUS, // clear RT status
                    R_STATUS_MAX_RT
                    );
            lcdPrint("Sent ");
            lcdRefresh();
            send_key(0x04,myctr++);
            lcdPrintln("a");
            lcdRefresh();
            // empty.
            lcdRefresh();
            while(getInputRaw()!=BTN_NONE){delayms(10);};
        };
        if(key==BTN_RIGHT|| key==BTN_LEFT){
            myctr=0;
            while(getInputRaw()!=BTN_NONE){delayms(10);};
            if(key==BTN_RIGHT)
                lineno++;
            if(key==BTN_LEFT)
                lineno--;
            if(lineno<1)
                lineno=1;
            n=read_chmac(lineno,chmac);
            if(n<0){
                lineno=1;
                read_chmac(lineno,chmac);
            };
            setup_send(chmac);
        };

//        ctr=buf[5]|buf[6]<<8;
        delayms(10);
        lcdRefresh();
    } while (key!=BTN_ENTER);

    while(getInputRaw()!=BTN_NONE){delayms(10);};
}

void setup_send(char*chmac){
    int i;

    lcdClear();
    lcdPrintln("Ch:MAC");
    for (i=0;i<=5;i++){
        lcdPrint(IntToStr(chmac[i],2,F_HEX));
        if(i==0)
            lcdPrint(":");
    };
    lcdPrintln("");
    lcdRefresh();

//    nrf_rcv_pkt_end();
    config.channel=chmac[0];
    memcpy(config.txmac,chmac+1,5);
    memcpy(config.mac0,chmac+1,5); // For receive of auto-ack packets
    nrf_config_set(&config);

    nrf_write_reg(R_FEATURE,
            R_FEATURE_EN_DPL // Enable dynamic payload length
            );

    nrf_write_reg(R_DYNPD,
            R_DYNPD_DPL_P0 // Enable dynamic payload length
            );

    nrf_write_reg(R_EN_AA,
            R_EN_AA_ENAA_P0 // Enable Enhanced ShockBurst[tm]
            );

    nrf_write_reg(R_SETUP_RETR,
            R_SETUP_RETR_ARD(500)| // 500us retransmission timeout
                                   /* 500us is minimum for 16-byte payloads */
            R_SETUP_RETR_ARC(1)   // 15(=max) retransmissions on fail of AA
            );

    nrf_write_reg(R_STATUS, // clear status
            R_STATUS_MAX_RT
            );

    nrf_write_reg(R_CONFIG,
            R_CONFIG_PWR_UP|  // Power on
            R_CONFIG_EN_CRC|  // CRC on, double byte
            R_CONFIG_CRCO
            );
}

#define PKTLEN 16
char send_key(char key,int ctr){
    unsigned char keypkt[PKTLEN];
    int i,k;

    // 1: setup key packet
    keypkt[ 0]=0x0a; // "device type"
    keypkt[ 1]=0x78; // packet type (0x78 keypress, 0x38: keyhold)
    keypkt[ 2]=0x00; // "model"
    keypkt[ 3]=0xF0; // ?
    keypkt[ 4]=0x00; // ctr: lo byte        *** "encryption" starts here
    keypkt[ 5]=0x00; // ctr: high byte
    keypkt[ 6]=0x43; // "flags"?
    keypkt[ 7]=0x00; // modifier key-state
    keypkt[ 8]=0x00; //
    keypkt[ 9]=0x00; // key[0]
    keypkt[10]=0x00; // key[1]
    keypkt[11]=0x00; // key[2]
    keypkt[12]=0x00; // key[3]
    keypkt[13]=0x00; // key[4]
    keypkt[14]=0x00; // key[5]
    keypkt[15]=0x00; // checksum            *** "encryption" ends here

    keypkt[4]=ctr&0xff;
    keypkt[9]=key;

    // 2: calc checksum
    k=0xff;
    /* calculate Checksum */
    for (i=0;i<PKTLEN;i++){
        k^=keypkt[i];
    };
    keypkt[15]=k;

    // 3: "encrypt"
    /* do "Encryption" */
    for (i=4;i<=PKTLEN;i++){
        keypkt[i]=keypkt[i]^config.txmac[(i-4)%5]; 
    };

    // 4: send
    return nrf_snd_pkt(PKTLEN,keypkt);

    // 5: wait for ack? / timing?
}


int get_line(FIL *filep, char * data, int len){
    unsigned int readbytes;
    int idx=0;
    char dummy[1];
    int res;

    while (1){
        res = f_read(filep, dummy, 1, &readbytes);

        if(res || readbytes<=0){
            return -1;
        };
        if(dummy[0] == 0x0a ){
            break;
        };
        if(idx <len){
            data[idx++]=dummy[0];
        };
    };
    return idx;
}

/* read a line from file */
#define LINELEN 14
signed char read_chmac(signed int lineno, char chmac[6]){
    FIL file;
    char * filename = "kbd.mac";
    char line[LINELEN];
    int i;
    int ctr=0;
    char nibble;
    int res;
    signed int bytes;

    for (i=0;i<6;i++)
        chmac[i]=0;

    res=f_open(&file, filename, FA_OPEN_EXISTING|FA_READ);
    if(res){
        return -1;
    };

    do{
        bytes=get_line(&file,line,LINELEN);
        if(bytes<=0)
            return -1;
    }while(--lineno>0);

    for(i=0;i<bytes;i++){ /* convert ascii-hex to binary */
        if(line[i]>='0' && line[i]<='9'){
            nibble=line[i]-'0';
        }else{
            line[i]&=0xdf;
            if(line[i]>='A' && line[i]<='F'){
                nibble=line[i]-'A'+10;
            }else{
                continue;
            };
        };
        chmac[ctr>>1]|=nibble<<(ctr%2?0:4);
        if(++ctr == 6*2)
            break;
    };

    f_close(&file);
    return 0;
}
