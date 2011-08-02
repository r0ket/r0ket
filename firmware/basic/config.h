#ifndef __BASICCONFIG_H_
#define __BASICCONFIG_H_

int readConfig(void);
int saveConfig(void);
void applyConfig(void);


struct CDESC {
    char *name;
    char value;
    char min;
    char max;
};

#define MAXNICK 20
extern struct CDESC the_config[];
extern char nickname[MAXNICK];
extern char nickfont[];

#define GLOBALversion      (the_config[0].value)
#define GLOBALprivacy      (the_config[1].value)
#define GLOBALdaytrig      (the_config[2].value)
#define GLOBALdaytrighyst  (the_config[3].value)
#define GLOBALdayinvert    (the_config[4].value)
#define GLOBALlcdbacklight (the_config[5].value)
#define GLOBALlcdmirror    (the_config[6].value)
#define GLOBALlcdinvert    (the_config[7].value)
#define GLOBALlcdcontrast  (the_config[8].value)
#define GLOBALflamemax     (the_config[9].value)
#define GLOBALflamemin     (the_config[10].value)
#define GLOBALflamespeed   (the_config[11].value)
#define GLOBALflamemaxw    (the_config[12].value)
#define GLOBALflameminw    (the_config[13].value)
#define GLOBALnickname     (nickname)
#define GLOBALnickfont     (nickfont)

#define GLOBAL(x) GLOBAL ## x

#endif
