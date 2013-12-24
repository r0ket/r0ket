#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/render.h"
#include "lcd/print.h"

#include "funk/nrf24l01p.h"
#include "filesystem/ff.h"

#include "usetable.h"

/**************************************************************************/

#define MAC_TABLE_SIZE 10
#define ENTRY_SIZE 6      // Channel + MAC

#define PACKETLEN   25

struct NRF_CFG config = {
    .channel= 81,
    .txmac= "\x0\x0\x0\x0\x0",
    .nrmacs=1,
    .mac0=  "\x55\x0\x0\x0\x0",
    .maclen ="\x19",
};

void shift_buffer_left(uint8_t *buffer, uint8_t n, uint8_t shift);
void shift_buffer_right(uint8_t *buffer, uint8_t n, uint8_t shift);
void configure_nrf(uint8_t channel);
void inc_channel(uint8_t *channel);
void dec_channel(uint8_t *channel);
void print_channel_mac(uint8_t channel, uint8_t *mac);
bool find_mac_in_packet(uint8_t *p);
void process_detected_mac(uint8_t channel, uint8_t *mac, uint8_t *mac_table, uint8_t *mac_entries);
//bool is_entry_known(uint8_t *entry);
bool is_entry_known(uint8_t *entry, uint8_t *mac_table, uint8_t mac_entries);
int memcmp(const void *av, const void *bv, size_t len);
void set_scan(bool *scan, bool value);
void write_mac_table(uint8_t *mac_table, uint8_t mac_entries);

void ram(void) {
    uint8_t channel=29;
    char key;
    unsigned char p[PACKETLEN];
    uint8_t mac_table[MAC_TABLE_SIZE * ENTRY_SIZE];
    uint8_t mac_entries = 0;
    #define LOOPY   50
    uint8_t loopy = 0;
    bool scan;

    lcdClear();
    lcdPrintln("Key-Scan:");
    lcdPrintln("");
    lcdPrintln("Last Ch:MAC");
    lcdPrintln("");
    lcdPrintln("");
    lcdPrintln("");
    lcdPrintln("");
    lcdPrintln("U:Save D:Scan");
    configure_nrf(channel);
    set_scan(&scan, true);

      while(true) {
        key = getInputRaw();

        if(key!=BTN_NONE){
            if ( key == BTN_RIGHT ){
                inc_channel(&channel);
            } else if ( key == BTN_LEFT ){
                dec_channel(&channel);
            } else if ( key == BTN_UP ){
                write_mac_table(mac_table, mac_entries);
            } else if ( key == BTN_DOWN ){
                set_scan(&scan, !scan);
            } else if ( key == BTN_ENTER ){
                return;
            }
            delayms(200);
        }
        
        if(scan){
            if(loopy++ == LOOPY){
                inc_channel(&channel);
                loopy = 0;
            }
        }

        int n;
        n = nrf_rcv_pkt_poll(sizeof(p), p);

        if(n==0 || n==-1 || n==-2){
            // No packet, or packet to large
            delayms(20);
            continue;
        }

        if(n!=sizeof(p)){
            lcdPrint("<-Err n=");
            lcdPrint(IntToStr(n,3,0));
            lcdPrint("->");
            lcdRefresh();
            continue;
        };

        if(find_mac_in_packet(p)){
            process_detected_mac(channel, p, mac_table, &mac_entries);
        }
    };
}

void shift_buffer_left(uint8_t *buf, uint8_t n, uint8_t shift)
{
    while(shift--) {
        uint8_t i;
        for (i = 0; i < n; i++){
            buf[i]=(buf[i] << 1) | (buf[i+1] >> 7);
        };
        buf[n-1] &= 0xFE;
    }
}

void shift_buffer_right(uint8_t *buf, uint8_t n, uint8_t shift)
{
    while(shift--) {
        int8_t i;
        for (i = n - 1; i >= 0; i--){
            buf[i]=(buf[i] >> 1) | (buf[i-1] << 7);
        };
        
        // Fill with pattern
        buf[0] &= 0x7F;
        if((buf[0] & 0x40) == 0) {
            buf[0] |= 0x80;
        }
    }
}

void configure_nrf(uint8_t channel)
{
    lcdSetCrsr(0,8);
    lcdPrint("Ch: ");
    lcdPrint(IntToStr(channel,3,0));
    lcdPrintln("");
    lcdRefresh();

    config.channel=channel;
    nrf_config_set(&config);

    nrf_write_reg(R_SETUP_AW,0 /*R_SETUP_AW_2*/);
    nrf_rcv_pkt_start(0);   //Do not enable the CRC
}

void inc_channel(uint8_t *channel)
{
    if ( *channel < 99 ){
        (*channel)++;
    }else{
        *channel = 0;
    }
    configure_nrf(*channel);
}

void dec_channel(uint8_t *channel)
{
    if ( *channel > 0 ){
        (*channel)--;
    }else{
        *channel = 99;
    }
    configure_nrf(*channel);
}

void process_detected_mac(uint8_t channel, uint8_t *mac, uint8_t *mac_table, uint8_t *mac_entries)
{
    uint8_t entry[ENTRY_SIZE];
    lcdSetCrsr(0,8*3);
    print_channel_mac(channel, mac);

    entry[0]= channel;
    memcpy(entry+1, mac, ENTRY_SIZE-1);
    
    if(*mac_entries < MAC_TABLE_SIZE){
        if(!is_entry_known(entry, mac_table, *mac_entries)){
            memcpy(mac_table + (*mac_entries) * ENTRY_SIZE, entry, sizeof(entry));
            (*mac_entries)++;
            lcdSetCrsr(0,8*4);
            lcdPrint("Found: ");
            lcdPrint(IntToStr(*mac_entries,2,0));
        }
    }
    lcdRefresh();
}

void print_channel_mac(uint8_t channel, uint8_t *mac)
{
    lcdPrint(IntToStr(channel,2,0));
    lcdPrint(":");
    lcdPrint(IntToStr(mac[0],2,F_HEX));
    lcdPrint(IntToStr(mac[1],2,F_HEX));
    lcdPrint(IntToStr(mac[2],2,F_HEX));
    lcdPrint(IntToStr(mac[3],2,F_HEX));
    lcdPrint(IntToStr(mac[4],2,F_HEX));
}

bool is_entry_known(uint8_t *entry, uint8_t *mac_table, uint8_t mac_entries)
{
    int i;
    for(i=0; i<mac_entries; i++){
        if(memcmp(mac_table+i*ENTRY_SIZE, entry, ENTRY_SIZE) == 0){
            return true;
        }
    }
    return false;
}

bool find_mac_in_packet(uint8_t *p)
{
    unsigned char buf[PACKETLEN];
    uint8_t shift;
    uint8_t dir;
    //static uint8_t count = 0;

    // Shift the buffer around a little bit to see if it
    // matches a key press

    for(dir = 0; dir < 2; dir++) {
        for(shift = 0; shift < 4; shift++) {
            memcpy(buf, p, sizeof(buf));
            
            // Compensate for additional preamble bits
            // or bits of the MAC which have been detected
            // as preamble.
            if(dir == 0) {
                shift_buffer_left(buf, sizeof(buf), shift);
            } else {
                shift_buffer_right(buf, sizeof(buf), shift);
            }

            // we need to shift the payload one bit
            // to the left as there is a 9 bit
            // field in the shockburst+ header
            
            // Byte number 6 contains one bit
            // of the PCF as the MSB and 7 bit
            // of the first payload byte.
            //
            // Byte number 6 + 16 contains 7 bit
            // of the checksum
            //
            // Byte number 6 + 17 contains the LSB
            // of the checksum as MSB.
            shift_buffer_left(buf + 6, 17, 1);
            
            // We expect the MAC at byte number
            // 6 + 10 to 6 + 14
            //
            // The mac is with the following byte order
            // at the beginning: 4 3 2 1 0
            //
            // We expect the following byte order of the
            // mac in the paylod:
            // 1 2 3 4 0

            if(buf[6 + 10 + 0] == buf[3] &&
                    buf[6 + 10 + 1] == buf[2] &&
                    buf[6 + 10 + 2] == buf[1] &&
                    buf[6 + 10 + 3] == buf[0] &&
                    buf[6 + 10 + 4] == buf[4]) {
                // This originated from a keyboard
                
                // Copy the MAC in the correct order
                // to the buffer
                p[0] = buf[4];
                p[1] = buf[3];
                p[2] = buf[2];
                p[3] = buf[1];
                p[4] = buf[0];

                //lcdPrint(IntToStr(count++&0x0F,1,F_HEX));
                //lcdPrint(IntToStr(dir,1,F_HEX));
                //lcdPrint(IntToStr(shift,1,F_HEX));
                return true;
            }
        }
    }
    return false;
}

void set_scan(bool *scan, bool value)
{
    *scan = value;
    lcdSetCrsr(0,8*6);
    lcdPrint("Scan:");
    lcdPrintln(IntToStr(*scan,1,0));
    lcdRefresh();
}

void write_mac_table(uint8_t *mac_table, uint8_t mac_entries)
{
    int res;
    FIL file;
    UINT writebytes;
    int i,j;

    f_open(&file, "KBD.MAC", FA_OPEN_ALWAYS|FA_WRITE);
    if(res){
        lcdSetCrsr(0,8*5);
        lcdPrint("ERR OPEN");
        lcdRefresh();
        return;
    }
    //f_lseek(&file, f_size(&file));

    char buf[ENTRY_SIZE*2+1+1];
    buf[2] = ' ';
    buf[sizeof(buf)-1] = '\n';

    for(i=0; i<mac_entries; i++) {
        uint8_t *entry = mac_table + i * ENTRY_SIZE;

        memcpy(buf, IntToStr(entry[0],2,F_HEX),2);
        memcpy(buf+3, IntToStr(entry[1],2,F_HEX),2);
        memcpy(buf+5, IntToStr(entry[2],2,F_HEX),2);
        memcpy(buf+7, IntToStr(entry[3],2,F_HEX),2);
        memcpy(buf+9, IntToStr(entry[4],2,F_HEX),2);
        memcpy(buf+11, IntToStr(entry[5],2,F_HEX),2);

        res = f_write(&file, buf, sizeof(buf), &writebytes);
        if(res){
            lcdSetCrsr(0,8*5);
            lcdPrint("ERR WRITE");
            lcdRefresh();
            return;
        }
    }
    res=f_close(&file);
    if(res){
        lcdSetCrsr(0,8*5);
        lcdPrint("ERR CLOSE");
        lcdRefresh();
        return;
    } 
    
    lcdSetCrsr(0,8*5);
    lcdPrint("KBD.MAC saved");
    lcdRefresh();
    delayms(200);
}

#if 1
int memcmp(const void *av, const void *bv, size_t len)
{
  const unsigned char *a = av;
  const unsigned char *b = bv;
  size_t i;

  for (i=0; i<len; i++) 
  {
    if (a[i] != b[i]) 
    {
      return (int)(a[i] - b[i]);
    }
  }
  return 0;
}
#endif
