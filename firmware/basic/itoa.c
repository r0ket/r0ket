#include "basic/basic.h"

#define LEN 10
const char* IntToStrX(unsigned int num, unsigned int mxlen){
	static char s[LEN+1];
	int len;
	s[LEN]=0;
	for (len=(LEN-1);len>=(LEN-mxlen);len--){
		s[len]=(num%16)+'0';
		if(s[len]>'9')
			s[len]+='A'-'9'-1;
		num/=16;
	};
	return &s[len+1];
}
#undef LEN

#define LEN 11
const char* IntToStr(int num, unsigned int mxlen, char flag){
	static char s[LEN+1];
	int len;
	s[LEN]=0;
	char neg=0;

	if(num<0){
		num=-num;
		neg=1;
	};

	for (len=(LEN-1);len>=(LEN-mxlen);len--){
		s[len]=(num%10)+'0';
		num/=10;
	};
    len++;

    if(!(flag&F_LONG)){
        while(s[len]=='0' && len < (LEN-1))
            len++;
    }else if(!(flag&F_ZEROS)){
        int x=len;
        while(s[x]=='0' && x < (LEN-1)){
            s[x]=' ';
            x++;
        };
    }

	if(neg==1){
        len--;
        s[len]='-';
    }else if(flag&F_SPLUS){
        len--;
        s[len]='+';
    }else if(flag&F_SSPACE){
		s[len]=' ';
    };
	return &s[len];
}
#undef LEN
