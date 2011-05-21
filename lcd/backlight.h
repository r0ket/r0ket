#ifndef __BACKLIGHT_H_
#define __BACKLIGHT_H_ 1

void backlightInit(void);
int backlightSetBrightness(uint32_t percentage);
uint32_t backlightGetBrightness(void);

#endif /* __BACKLIGHT_H_ */
