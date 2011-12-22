#include <string.h>
#include "nrf24l01p.h"
#include "filetransfer.h"
#include "rftransfer.h"
#include "basic/basic.h"
#include "basic/xxtea.h"
#include "filesystem/ff.h"
#include "lcd/print.h"


//TODO: use a proper MAC to sign the message
int filetransfer_send(uint8_t *filename, uint16_t size,
                uint8_t *mac, uint32_t const k[4])
{
    uint8_t buf[MAXSIZE];
    FIL file;
    FRESULT res;
    UINT readbytes;


    if( size > MAXSIZE )
        return 1;           //File to big
 
    res=f_open(&file, (const char*)filename, FA_OPEN_EXISTING|FA_READ);
    if( res )
        return res;

    //res = f_read(&file, (char *)buf, size, &readbytes);
    for(uint16_t i=0; i<MAXSIZE; i++)
        buf[i] = 0;

    res = f_read(&file, (char *)buf, MAXSIZE, &readbytes);
    size = readbytes;

    if( res )
        return res;
    if( size != readbytes)
        return 1;           //Error while reading
    
    uint16_t wordcount = (size+3)/4;
    //uint8_t macbuf[5];

    uint8_t metadata[32];
    if( strlen((char*)filename) < 20 )
        strcpy((char*)metadata, (char*)filename);
    else
        return 1;           //File name too long
    metadata[20] = size >> 8;
    metadata[21] = size & 0xFF;

    //nrf_get_tx_max(5,macbuf);

    //nrf_set_tx_mac(5, mac); 
    nrf_snd_pkt_crc_encr(32, metadata, k); 
    delayms(20);
    xxtea_encode_words((uint32_t *)buf, wordcount, k);
    rftransfer_send(wordcount*4, buf);
    //nrf_set_tx_mac(5, macbuf);
    return 0;
}

int filetransfer_receive(uint8_t *mac, uint32_t const k[4])
{
    uint8_t buf[MAXSIZE+1];
    uint16_t size;
    uint8_t n;

    UINT written = 0;
    FIL file;
    FRESULT res;
    //uint8_t macbuf[5];
    //nrf_get_rx_max(0,5,macbuf);

    uint8_t metadata[32];

    //nrf_set_rx_mac(0, 32, 5, mac);
    n = nrf_rcv_pkt_time_encr(3000, 32, metadata, k);
    if( n != 32 )
        return 1;       //timeout
    //nrf_set_rx_mac(0, 32, 5, macbuf);
    //lcdPrintln("got meta"); lcdRefresh();
    metadata[19] = 0; //enforce termination
    size = (metadata[20] << 8) | metadata[21];

    if( size > MAXSIZE ) {lcdPrintln("too big"); lcdRefresh(); while(1);}
    if( size > MAXSIZE ) return 1; //file to big
    //if(fileexists(metadata)) return 1;   //file already exists
    
    //lcdPrint("open"); lcdPrintln((const char*)metadata); lcdRefresh();
    res = f_open(&file, (const char*)metadata, FA_OPEN_ALWAYS|FA_WRITE);

    //lcdPrintln("file opened"); lcdRefresh();
    //if( res ) {lcdPrintln("res"); lcdPrint(f_get_rc_string(res)); lcdRefresh(); while(1);}
    if( res ){ lcdPrintln("file error"); lcdRefresh(); while(1);}
    if( res )
        return res;
    
    uint16_t wordcount = (size+3)/4;
    
    //nrf_set_rx_mac(0, 32, 5, mac);
    //lcdPrintln("get file"); lcdRefresh();
    int fres = rftransfer_receive(buf, wordcount*4, 1000);
    if( fres == -1 ){
        lcdPrintln("checksum wrong");
    }else if( fres == -2 ){
        lcdPrintln("timeout");
    }else{
        //lcdPrintln("got file");
    }
    lcdRefresh();
    if( fres < 0 )
        return 1;
    //nrf_set_rx_mac(0, 32, 5, macbuf);

    xxtea_decode_words((uint32_t *)buf, wordcount, k);
    
    res = f_write(&file, buf, size, &written);
    f_close(&file);
    if( res )
        return res;
    if( written != size )
        return 1;           //error while writing
    lcdClear();
    lcdPrintln("Received"); lcdPrintln((const char*)metadata); lcdRefresh();

    return 0;
}

