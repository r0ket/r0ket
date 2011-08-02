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
#include "xxtea.h"

// default block size

void hexkey(char *string, uint32_t k[4]);

int main(int argc, char *argv[]) {
  FILE *fp;
  FILE *ofp;

  char *prog;
  char c;			/* for getopt */
  uint32_t k[4];    /* key */
  uint8_t *buf;

  /* Default values */
  char verbose=0;	// be silent
  k[0]=0; k[1]=0; k[2]=0; k[3]=0;
  char *outfile=NULL; // outfile == infile
  int decrypt=0;
  int encrypt=0;
  int sign=0;

  /* init section */
  prog=argv[0];
  if(!prog)prog="xxtea";
  if(strrchr(prog,'/')){
	  prog=strrchr(argv[0],'/');
	  prog++;
  }

  /* The big getopt loop */
  while ((c = getopt(argc, argv, "vhesdk:o:")) != EOF)
	  switch (c) {
		  case 'v':
			  verbose++;
			  break;

		  case 'd':
			  decrypt=1;
			  break;

          case 'e':
              encrypt=1;
              break;

          case 's':
              sign=1;
              break;

		  case 'k':
              hexkey(optarg, k);
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
"-d           Decrypt file\n"
"-e           Encrypt file\n"
"-s           Sign file\n\n"
"-o file      Output to <file>. (Default: overwrite input file)\n"
"-k key       128bit hex key.\n"
"-h           This help\n\n"
"\n",prog);
			  exit(255);

	  }

  argc -= optind; argv += optind;

  if (argc !=1){
	  fprintf(stderr,"Error: No filename given!\n");
	  exit(254);
  };

  if( !(decrypt||encrypt||sign) ){
      fprintf(stderr, "No operation specified!\n");
	  exit(254);
  }

  if( decrypt && (encrypt || sign) ){
      fprintf(stderr, "Can not decrypt and sign or decrypt and encrypt!\n");
	  exit(254);
  }
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

  fseek(fp, 0L, SEEK_END);
  int filesize = ftell(fp);
  fseek(fp, 0L, SEEK_SET);

  if (verbose)
    fprintf(stderr,"file size=%d\n", filesize);
  
  int words = (filesize+3)/sizeof(uint32_t);
  int bytes = sizeof(uint32_t)*words;

  if (verbose)
    fprintf(stderr,"byte count=%d word count=%d\n", bytes, words);
  

  buf=malloc(bytes);

  if(!buf){
      fprintf(stderr,"Error: malloc() failed.\n");
      exit(253);
  };

  if (verbose)
      fprintf(stderr,"Key: %08x %08x %08x %08x\n",k[0],k[1],k[2],k[3]);

  if( sign ){
      if (verbose)
          fprintf(stderr,"Signing: ");
      memset(buf, 0, bytes);
	  int cnt = fread(buf,sizeof(*buf),filesize,fp);
      uint32_t mac[4];
      xxtea_cbcmac(mac, (uint32_t*)buf, words, k);

      if (verbose)
        fprintf(stderr,"MAC: %08x %08x %08x %08x\n",mac[0],mac[1],mac[2],mac[3]);

      if(!outfile) // in-place crypting...
          if( fseek(fp, 0L, SEEK_SET) != 0){
              fprintf(stderr, "Error: Seek failed\n");
              exit(253);
          }

      if (fwrite(buf,sizeof(*buf),bytes,ofp) != bytes){
          fprintf(stderr, "Error: write failed\n");
          exit(253);
      }
      if (fwrite(mac,sizeof(*mac),4,ofp) != 4*sizeof(*mac)){
          fprintf(stderr, "Error: write failed\n");
          exit(253);
      }
      if(verbose) fprintf(stderr,".\n");
  }

  if( encrypt ){
    int cnt, block;
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

        //btea(buf, decrypt?-cnt:cnt, k);

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
  }
  if(verbose)
      fprintf(stderr,"done\n");

  fclose(fp);

  if(outfile)
      fclose(ofp);
  
  return 0;
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
