/* simple XXTEA en/decrypt utility
 * 
 * BSD Licence
 *
 * btea function is from 
 * <https://secure.wikimedia.org/wikipedia/en/wiki/XXTEA#Reference_code>
 *
 * (c) by Sec <sec@42.org> 6/2011
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>
#include <getopt.h>

// default block size

void btea(uint32_t *v, int n, uint32_t const k[4]);
void hexkey(char *string, uint32_t k[4]);

int main(int argc, char *argv[]) {
  FILE *fp;
  FILE *ofp;

  char *prog;
  char c;			/* for getopt */
  uint32_t k[4];    /* key */
  uint32_t *buf;

  /* Default values */
  char verbose=0;	// be silent
  k[0]=0; k[1]=0; k[2]=0; k[3]=0;
  char block=16;
  char *outfile=NULL; // outfile == infile

  /* init section */
  prog=argv[0];
  if(!prog)prog="xxtea";
  if(strrchr(prog,'/')){
	  prog=strrchr(argv[0],'/');
	  prog++;
  }

  /* The big getopt loop */
  while ((c = getopt(argc, argv, "vhk:b:o:")) != EOF)
	  switch (c) {
		  case 'v':
			  verbose++;
			  break;

		  case 'k':
              hexkey(optarg, k);
			  break;

		  case 'b':
			  block=atoi(optarg);
			  break;

		  case 'o':
			  outfile=optarg;
			  break;

		  case 'h':
		  default:
			  fprintf(stderr, "Usage: %s [options] filename\n\n"
"This program en/decrypts a file with the XXTEA algorithm\n"
"\n\n"
"-v           Be verbose (-v -v = even more)\n"
"-o file      Output to <file>. (Default: overwrite input file)\n"
"-k key       128bit hex key. [Not yet implemented]\n"
"-b block     Set blocksize. (Default: file size)\n"
"-h           This help\n\n"
"\n",prog);
			  exit(255);

	  }

  argc -= optind; argv += optind;

  if (argc !=1){
	  fprintf(stderr,"Error: No filename given!\n");
	  exit(254);
  };

  if(outfile){
      if ((fp = fopen(argv[0],"rb")) == NULL){
          fprintf(stderr,"Error: Can't open file %s\n",argv[0]);
          exit(253);
      }
      if ((ofp = fopen(outfile,"wb")) == NULL){
          fprintf(stderr,"Error: Can't open file %s\n",argv[0]);
          exit(253);
      }
  }else{
      if ((fp = fopen(argv[0],"r+b")) == NULL){
          fprintf(stderr,"Error: Can't open file %s\n",argv[0]);
          exit(253);
      }
      ofp=fp;
  };

  if(block==0){
      fseek(fp, 0L, SEEK_END);
      block = ftell(fp)/sizeof(*buf); // XXX: padding!
      fseek(fp, 0L, SEEK_SET);
  };
  buf=malloc(sizeof(*buf)*block);

  if(!buf){
      fprintf(stderr,"Error: malloc() failed.\n");
  };

  if (verbose)
      fprintf(stderr,"Key: %08x %08x %08x %08x\n",k[0],k[1],k[2],k[3]);

  int cnt;
  if (verbose)
      fprintf(stderr,"Encrypting: ");

  do{
	  cnt=fread(buf,sizeof(*buf),block,fp); // XXX: deal with non-block-sized?

      if(cnt<0){
		  fprintf(stderr, "Error: read failed\n");
		  exit(253);
      };

      if(verbose)
          fprintf(stderr,"cnt=%d:",cnt);
/*      if(cnt%sizeof(*buf)!=0){
          fprintf(stderr,"Whoops. needs padding: cnt=%d, mod=%d\n",cnt,cnt%sizeof(*buf));
      }; */

      btea(buf, cnt, k);

      if(!outfile) // in-place crypting...
          if (fseek(fp,-cnt*sizeof(*buf),SEEK_CUR) != 0){
              fprintf(stderr, "Error: Seek failed\n");
              exit(253);
          }

      if (fwrite(buf,sizeof(*buf),cnt,ofp) != cnt){
          fprintf(stderr, "Error: CRC write failed\n");
          exit(253);
      }
      if(verbose) fprintf(stderr,".\n");
  }while(cnt==block);

  if(verbose)
      fprintf(stderr,"done\n");

  fclose(fp);

  if(outfile)
      fclose(ofp);
  
  return 0;
}

#define DELTA 0x9e3779b9
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (k[(p&3)^e] ^ z)))

void btea(uint32_t *v, int n, uint32_t const k[4]) {
    uint32_t y, z, sum;
    unsigned p, rounds, e;
    if (n > 1) {          /* Coding Part */
        rounds = 6 + 52/n;
        sum = 0;
        z = v[n-1];
        do {
            sum += DELTA;
            e = (sum >> 2) & 3;
            for (p=0; p<n-1; p++) {
                y = v[p+1]; 
                z = v[p] += MX;
            }
            y = v[0];
            z = v[n-1] += MX;
        } while (--rounds);
    } else if (n < -1) {  /* Decoding Part */
        n = -n;
        rounds = 6 + 52/n;
        sum = rounds*DELTA;
        y = v[0];
        do {
            e = (sum >> 2) & 3;
            for (p=n-1; p>0; p--) {
                z = v[p-1];
                y = v[p] -= MX;
            }
            z = v[n-1];
            y = v[0] -= MX;
        } while ((sum -= DELTA) != 0);
    }
}

void hexkey(char *string, uint32_t k[4]){
    int idx=0;
    int kidx=0;
    int kctr=0;
    int value;
    char ch;

    while ((ch=string[idx++])!=0){
        if(ch == ' ')
            continue;
        if(ch == '\t')
            continue;

        if (ch >= '0' && ch <= '9')
            value = (ch - '0');
        else if (ch >= 'A' && ch <= 'F')
            value = (ch - 'A' + 10);
        else if (ch >= 'a' && ch <= 'f')
            value = (ch - 'a' + 10);
        else
            continue;


        k[kidx]=(k[kidx]<<4)+value;
        kctr++;
        if(kctr>=8){
            kctr=0;
            kidx++;
            if(kidx>4)
                return;
        };
    };
}
