#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/backlight.h"
#include "lcd/allfonts.h"

#include "filesystem/ff.h"
#include "filesystem/diskio.h"

#include "usb/usbmsc.h"

#include "core/ssp/ssp.h"

void incBacklight(void);
void decBacklight(void);
void gotoISP(void);
void fs_list(void);
void fs_create(void);
void fs_read(void);
void fs_format(void);
void fs_status(void);
void fs_mount(void);

/**************************************************************************/

struct MENU_DEF {
    char *text;
    void (*callback)(void);
};

typedef const struct MENU_DEF * menuentry;

struct MENU {
    char *title;
    menuentry *entries;
};

const struct MENU_DEF menu_incBL =  {"Backlight++", &incBacklight};
const struct MENU_DEF menu_decBL =  {"Backlight--", &decBacklight};
const struct MENU_DEF menu_ISP =    {"Invoke ISP",  &gotoISP};
const struct MENU_DEF menu_status = {"FS Status",   &fs_status};
const struct MENU_DEF menu_mount =  {"FS Mount",    &fs_mount};
const struct MENU_DEF menu_list =   {"FS List",     &fs_list};
const struct MENU_DEF menu_create = {"FS Create",   &fs_create};
const struct MENU_DEF menu_format = {"FS format",   &fs_format};
const struct MENU_DEF menu_read   = {"FS read",     &fs_read};
const struct MENU_DEF menu_usb   =  {"USB mount",   &usbMSCInit};
const struct MENU_DEF menu_usboff   =  {"USB unmount", &usbMSCOff};
const struct MENU_DEF menu_nop =    {"---",   NULL};

static menuentry menu[] = {
    &menu_mount,
    &menu_status,
    &menu_list,
    &menu_create,
    &menu_read,
    &menu_nop,
    &menu_format,
    &menu_nop,
    &menu_usb,
    &menu_usboff,
    &menu_nop,
    &menu_ISP,
    &menu_incBL,
    &menu_decBL,
    NULL,
};

static const struct MENU mainmenu = {"Mainmenu", menu};

void handleMenu(const struct MENU *the_menu) ;

void main_fs(void) {

    backlightInit();

    font_direction = FONT_DIR_LTR; // LeftToRight is the default

    while (1) {
        lcdDisplay(0);
        delayms(10);

        lcdFill(0); // clear display buffer

        handleMenu(&mainmenu);
        gotoISP();
    }

    return;
}

void handleMenu(const struct MENU *the_menu) {
    uint8_t back = 0;
    int8_t menuselection = 0;
    uint8_t numentries = 0;
    uint8_t visible_lines = 0;
    uint8_t current_offset = 0;

    if (the_menu == NULL) return;

    font = &Font_7x8;

    for (numentries = 0; the_menu->entries[numentries] != NULL; numentries++);

    visible_lines = RESY/font->u8Height;

    if (visible_lines < 2) return;

    visible_lines--; // subtract title line

    while (!back) {
        uint8_t line = 0;

        lcdFill(0); // clear display buffer

        DoString(0, line, the_menu->title);
        line += font->u8Height;

        for (uint8_t i = current_offset; i < (visible_lines + current_offset) && i < numentries; i++) {
            DoString(14, line, the_menu->entries[i]->text);
            if (i == menuselection) {
                DoString(0, line, "* ");
            }
            line += font->u8Height;
        }

        lcdDisplay(0);

        switch (getInput()) {
            case BTN_UP:
                menuselection--;
                if (menuselection < current_offset) {
                    if (menuselection < 0) {
                        menuselection = numentries-1;
                        current_offset = ((numentries-1)/visible_lines) * visible_lines;
                    } else {
                        current_offset -= visible_lines;
                    }
                }
                break;
            case BTN_DOWN:
                menuselection++;
                if (menuselection > (current_offset + visible_lines-1) || menuselection >= numentries) {
                    if (menuselection >= numentries) {
                        menuselection = 0;
                        current_offset = 0;
                    } else {
                        current_offset += visible_lines;
                    }
                }
                break;
            case BTN_LEFT:
                return;
            case BTN_RIGHT:
                if (the_menu->entries[menuselection]->callback!=NULL)
                    the_menu->entries[menuselection]->callback();
                break;
            case BTN_ENTER:
                DoString(0,0,"Called....");
                lcdDisplay(0);
                lcdFill(0);
                if (the_menu->entries[menuselection]->callback!=NULL)
                    the_menu->entries[menuselection]->callback();
                lcdDisplay(0);
                while (getInput()==BTN_NONE) delayms(10);

                break;
            default:
                /* no button pressed */
                break;
        }

    }

    return;
}

void incBacklight(void) {
    uint32_t brightness = backlightGetBrightness();
    if (brightness < 100) {
        backlightSetBrightness(brightness + 10);
    }
}

void decBacklight(void) {
    uint32_t brightness = backlightGetBrightness();
    if (brightness > 0) {
        backlightSetBrightness(brightness - 10);
    }
}


void tick_fs(void){
    static int foo=0;
    static int toggle=0;
	if(foo++>50){
        toggle=1-toggle;
		foo=0;
        gpioSetValue (RB_LED0, toggle); 
	};
    return;
};

void gotoISP(void) {
    DoString(0,0,"Enter ISP!");
    lcdDisplay(0);
    ISPandReset(5);
}

void put_rc_y (FRESULT rc, int y) {
	const TCHAR *p =
		_T("OK\0DISK_ERR\0INT_ERR\0NOT_READY\0NO_FILE\0NO_PATH\0INVALID_NAME\0")
		_T("DENIED\0EXIST\0INVALID_OBJECT\0WRITE_PROTECTED\0INVALID_DRIVE\0")
		_T("NOT_ENABLED\0NO_FILE_SYSTEM\0MKFS_ABORTED\0TIMEOUT\0LOCKED\0")
		_T("NOT_ENOUGH_CORE\0TOO_MANY_OPEN_FILES\0");
	FRESULT i;

	for (i = 0; i != rc && *p; i++) {
		while(*p++) ;
	}
    DoString(0,y,p);
}

void put_rc (FRESULT rc){
    put_rc_y(rc,0);
};


long p1, p2, p3;
FATFS FatFs[_VOLUMES];		/* File system object for logical drive */
//BYTE Buff[32768];			/* Working buffer */
FRESULT res;
FATFS *fs;				/* Pointer to file system object */
static const BYTE ft[] = {0, 12, 16, 32};

void fs_mount(void){
    put_rc(f_mount(0, &FatFs[0]));
};

void fs_status(void){
    int dx;
    int dy=8;

    DWORD clusters;

    res = f_getfree("0:", &clusters, &fs);
    put_rc(res);
    if(res){
        return;
    };

    dx=DoString(0,dy,"FAT type:");
    dx=DoInt(dx,dy,ft[fs->fs_type & 3]);
    dy+=8;
    dx=DoString(0,dy,"noFats:");
    dx=DoInt(dx,dy,fs->n_fats);

    dy+=8;
    dx=DoString(0,dy,"s/c");
    dx=DoInt(dx,dy,fs->csize);
    dx=DoString(dx,dy,"*");
    dx=DoInt(dx,dy,clusters);

    dy+=8;
    dx=DoString(0,dy,"maxE");
    dx=DoInt(dx,dy,fs->n_rootdir);
};

void fs_format(void){
    res=f_mkfs(0,1,0);
    put_rc(res);
};

void fs_list(void){
    DIR dir;                /* Directory object */
    FILINFO Finfo;

    int yctr=8;

    res = f_opendir(&dir, "0:");
    put_rc(res);
    if(res){
        return;
    };

    for(;;) {
        res = f_readdir(&dir, &Finfo);
        if ((res != FR_OK) || !Finfo.fname[0]) break;

/*
_tprintf(_T("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  "),
        (Finfo.fattrib & AM_DIR) ? 'D' : '-',
        (Finfo.fattrib & AM_RDO) ? 'R' : '-',
        (Finfo.fattrib & AM_HID) ? 'H' : '-',
        (Finfo.fattrib & AM_SYS) ? 'S' : '-',
        (Finfo.fattrib & AM_ARC) ? 'A' : '-',
        (Finfo.fdate >> 9) + 1980, (Finfo.fdate >> 5) & 15, Finfo.fdate & 31,
        (Finfo.ftime >> 11), (Finfo.ftime >> 5) & 63, Finfo.fsize);
_tcscpy(pool, Finfo.fname);
*/
        if (Finfo.fattrib & AM_DIR){
            DoString(0,yctr,"D");
        }else{
            DoString(0,yctr,"-");
        };


        int dx=10;
        dx=DoString(dx,yctr,Finfo.fname);
        dx=DoString(dx,yctr," <");
        dx=DoInt(dx,yctr,Finfo.fsize);
        dx=DoString(dx,yctr,">");

        yctr+=8;
    }
    DoString(0,yctr,"<done>");
};

FIL file[2];            /* File objects */

void fs_create(void){
    BYTE Buff[10];
    UINT written;

    put_rc(f_open(&file[0], "test.r0", FA_OPEN_ALWAYS|FA_WRITE));
    if(res){
        return;
    };

    Buff[0]=42;
    Buff[1]=23;
    res = f_write(&file[0], Buff, 2, &written);
    put_rc_y(res,8);
    if(res){
        return;
    };

    int dx;
    dx=DoString(0,16,"wrote:");
    dx=DoInt(dx,16,written);
    DoString(dx,16,"b");

    put_rc_y(f_close(&file[0]),24);
    if(res){
        return;
    };
};

void fs_read(void){
    BYTE Buff[10];
    UINT readbytes;

    put_rc(f_open(&file[0], "test.r0", FA_OPEN_EXISTING|FA_READ));
    if(res){
        return;
    };

    Buff[0]=0;
    Buff[1]=0;
    Buff[2]=0;
    Buff[3]=0;

    res = f_read(&file[0], Buff, 2, &readbytes);
    put_rc_y(res,8);
    if(res){
        return;
    };

    int dx;
    dx=DoString(0,16,"read: ");
    DoInt(dx,16,readbytes);

    DoIntX(0,24,((uint32_t*)Buff)[0]); // Hackety-Hack

    put_rc_y(f_close(&file[0]),32);
    if(res){
        return;
    };
};

