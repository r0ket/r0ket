#ifndef _UTIL_H
#define _UTIL_H 1

void fsInit();
void fsReInit();
int readFile(char * filename, char * data, int len);
int readTextFile(char * filename, char * data, int len);
int writeFile(char * filename, char * data, int len);

#endif /* _UTIL_H */
