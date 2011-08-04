#ifndef __DISPLAYIMG_H_
#define __DISPLAYIMG_H_

void lcdLoadImage(char *file);
void lcdSaveImage(char *file);
uint8_t lcdShowAnim(char *file, uint32_t framems);

#endif
