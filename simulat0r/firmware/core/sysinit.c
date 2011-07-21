#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

#include "core/lpc134x.h"

void testByte(void* addr) {
  fprintf(stderr,"Testing address %x : read ",addr);
  fprintf(stderr,"%x, write ",(int)(*((char*)addr)));
  ++(*((char*)addr));
  fprintf(stderr,"%x",(int)(*((char*)addr)));
  --(*((char*)addr));
  fprintf(stderr," OK\n");
}

void testMemoryHack(void* addr,long size) {
  fprintf(stderr,"Testing memory range %x - %x\n",addr,addr+size);
  for(void* p=addr; p<addr+size; p+=sysconf(_SC_PAGE_SIZE)) {
    testByte(p);  
  }
}

int setupMemoryHack(void* address, long size) {
  char *addr;
  int fd;
  struct stat sb;
  off_t offset, pa_offset;
  size_t length;
  ssize_t s;

  FILE *tempfil=tmpfile();
  fd = fileno(tempfil);
  ftruncate(fd,size);
  if (fd == -1)
    handle_error("open");
  
  if (fstat(fd, &sb) == -1)           /* To obtain file size */
    handle_error("fstat");
  
  offset = 0;
  pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
  /* offset for mmap() must be page aligned */
  
  if (offset >= sb.st_size) {
    fprintf(stderr, "offset is past end of file\n");
    exit(EXIT_FAILURE);
  }

  length = size;

  addr = mmap(address, length + offset - pa_offset, PROT_READ | PROT_WRITE,
	      MAP_PRIVATE, fd, pa_offset);
  if (addr == MAP_FAILED)
    handle_error("mmap");
  if(addr!=address) {
    fprintf(stderr, "mmap: wanted %x, got %x: ",address,addr);
    handle_error("mmap address discrepancy");
  }
  //  testMemoryHack(address,size);
  fprintf(stderr,"Range %x tested\n",addr);
}


void systemInit()
{
  //  setupMemoryHack((void*)0x40000000,0x1000000);
  //  setupMemoryHack((void*)0x50000000,10*1024*1024);
  //  setupMemoryHack((void*)0x10000000,10*1024*1024); 
  // systick stuff unmappable  setupMemoryHack((void*)0xe0000000,0x10000); 

#if 0
  printf("Test %d\n",SSP_SSP0CR0);
  printf("Test2 %d\n",++SSP_SSP0CR0);
  printf("Test3 pre %x\n",&SYSTICK_STRELOAD);
  //  printf("Test3 %d\n",++SYSTICK_STRELOAD);
#endif
}
