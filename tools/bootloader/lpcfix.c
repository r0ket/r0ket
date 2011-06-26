/* LPC CRP enabler / CRC fixer all-in-one
 * 
 * BSD Licence
 *
 * (c) by Sec <sec@42.org> 6/2011
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <getopt.h>

#define CRC_CNT 7
#define CRC_T uint32_t

#define CRP_OFF 0x2fc
#define CRP_T uint32_t
#define CRP_MAX 4

int main(int argc, char *argv[]) {
  FILE *fp;
  CRC_T buf[CRC_CNT];
  CRC_T crc = 0;
  CRP_T magic[]= { 0x0, 0x12345678, 0x87654321, 0x43218765, 0x4e697370};
  CRP_T crp;

  char *prog;
  char c;			/* for getopt */

  /* Default values */
  char verbose=0;	// be silent
  char do_crc= 0;	// do fix crc
  int  do_crp=-1;	// do not change crp

  /* init section */
  prog=argv[0];
  if(!prog)prog="lpcfix";
  if(strrchr(prog,'/')){
	  prog=strrchr(argv[0],'/');
	  prog++;
  }

  /* The big getopt loop */
  while ((c = getopt(argc, argv, "vcp:nh")) != EOF)
	  switch (c) {
		  case 'c':
			  do_crc = 1;
			  break;

		  case 'v':
			  verbose++;
			  break;

		  case 'p':
			  do_crp = atoi(optarg);
			  break;

		  case 'h':
		  default:
			  fprintf(stderr, "Usage: %s [options]\n\n"
"This program fixes the CRC field for LPC firmware and optionally\n"
"enables/disables the content read protection flags\n"
"\n\n"
"-n           Do not change CRC information\n"
"-v           Be verbose (-v -v = even more)\n"
"-c mode      Enable CRP (Content read protection) mode\n"
"-h           This help\n\n"
"CRP modes valid:\n"
"0            Disable CRP\n"
"1-3          Enable CRP 1-3\n"
"4            Enable NO_ISP\n"
"\n",prog);
			  exit(255);

	  }

  argc -= optind; argv += optind;

  if (argc !=1){
	  fprintf(stderr,"Error: No filename given!\n");
	  exit(254);
  };

  if(do_crp>CRP_MAX){
	  fprintf(stderr, "CRP %d is not allowed\n",do_crp);
	  exit(254);
  };

  if ((fp = fopen(argv[0],"r+b")) == NULL){
    fprintf(stderr,"Error: Can't open file %s\n",argv[0]);
    exit(253);
  }

  if (do_crc) {
	  if (fread(buf,sizeof(CRC_T),CRC_CNT,fp) != CRC_CNT){
		  fprintf(stderr,"Error: Read failed\n");
		  exit(253);
	  }

	  for (int idx=0; idx<CRC_CNT; idx++) {
		  crc += buf[idx];
	  }
	  crc = (~crc) + 1;

	  if(verbose)
		  printf("New CRC is:    0x%08x\n",crc);

	  if (fseek(fp,CRC_CNT*sizeof(CRC_T),SEEK_SET) != 0){
		  fprintf(stderr, "Error: Seek failed\n");
		  exit(253);
	  }

	  // Write CRC
	  if (do_crc)
		  if (fwrite(&crc,sizeof(CRC_T),1,fp) != 1){
			  fprintf(stderr, "Error: CRC write failed\n");
			  exit(253);
		  }
  };

  if (verbose){
	  if (fseek(fp,CRP_OFF,SEEK_SET) != 0){
		  fprintf(stderr, "Error: CRP Seek failed\n");
		  exit(253);
	  }
	  if (fread(&crp,sizeof(CRP_T),1,fp) != 1){
		  fprintf(stderr,"Error: CRP Read failed\n");
		  exit(253);
	  }
	  int idx;
	  for (idx=1; idx <= CRP_MAX; idx++) {
		  if(crp==magic[idx])
			  break;
	  }
	  if(idx>CRP_MAX)
		  idx=0;
	  printf("Curent CRP:    0x%08x (%d)\n", crp, idx);
  };

  if (do_crp!=-1){
	  crp=magic[do_crp];
	  if (fseek(fp,CRP_OFF,SEEK_SET) != 0){
		  fprintf(stderr, "Error: CRP Seek failed\n");
		  exit(253);
	  }
	  if (fwrite(&crp,sizeof(CRP_T),1,fp) != 1){
		  fprintf(stderr,"Error: CRP Write failed\n");
		  exit(253);
	  }
	  printf("New CRP value: 0x%08x\n", crp);
  };

  fclose(fp);
  
  return 0;
}

