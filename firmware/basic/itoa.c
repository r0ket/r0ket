#define LEN 32

const char* IntToStrX(unsigned int num, unsigned int mxlen){
	static char s[LEN+1];
	char * o=s;
	int len;
	s[LEN]=0;
	for (len=(LEN-1);len>=(LEN-mxlen);len--){
		s[len]=(num%16)+'0';
		if(s[len]>'9')
			s[len]+='A'-'9'-1;
		num/=16;
	};
	return &s[len+1];
};
