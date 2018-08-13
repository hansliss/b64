#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "hexdump.h"
#include "b64_func.h"

#define BUFSIZE (262144*8)

void usage(char *progname)
{
  fprintf(stderr, "Usage: %s [-d (decode)] [-h (hexdump)] [-f <file> | text ...]\n", progname);
}


int main(int argc, char *argv[])
{
  int decode=0, binary=1, o, r;
  static char tmpbuf[BUFSIZE], inbuf[BUFSIZE];
  FILE *infile=NULL;
  int n;
  while ((o=getopt(argc, argv, "dhf:"))!=-1) {
    switch (o)
      {
      case 'd':
	decode=1;
	break;
      case 'h':
	binary=0;
	break;
      case 'f':
	if (!(infile=fopen(optarg,"r"))) {
	  perror(optarg);
	  return -1;
	}
	break;
      default:
	usage(argv[0]);
	return -1;
	break;
      }
  }

  inbuf[0]='\0';
  if (infile) {
    if (decode) {
      while (fgets(tmpbuf, sizeof(tmpbuf), infile)) {
	while (strlen(tmpbuf) && isspace(tmpbuf[strlen(tmpbuf)-1])) tmpbuf[strlen(tmpbuf)-1]='\0';
	strcat(inbuf,tmpbuf);
      }
      n=strlen(inbuf);
    } else {
      n=fread(inbuf, 1, sizeof(inbuf), infile);
    }
    fclose(infile);
  } else {
    for (; optind<argc; optind++) {
      strcat(inbuf, argv[optind]);
    }
    n=strlen(inbuf);
  }

  if (decode) {
    if (!(r=b64_decode(inbuf, n, tmpbuf, sizeof(tmpbuf)))) {
      fprintf(stderr, "Decode failed.\n");
      return -2;
    }
  } else {
    if (!(r=b64_encode(inbuf, n, tmpbuf, sizeof(tmpbuf)))) {
      fprintf(stderr, "Encode failed.\n");
      return -2;
    }
  }

  if (binary) {
    fwrite(tmpbuf, 1, r, stdout);
    if (!decode) fputc('\n', stdout);
  } else {
    hexdump(stdout, tmpbuf, r);
  }

  return 0;
}
