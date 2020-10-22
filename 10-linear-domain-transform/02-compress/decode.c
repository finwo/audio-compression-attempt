#define _GNU_SOURCE

#include <arpa/inet.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "helpers.h"

int main(int argc, char *argv[]) {
  argparse(argc, argv);
  int i;

  int ssize = sizeof(signed short);
  int dsize = sizeof(double);

  signed short *ssamples = malloc(ssize * interval);

  unsigned short channel;
  unsigned short usample;
  double *dsamples = malloc(dsize * interval);

  while(!feof(stdin)) {

    // Read channel
    fread(&channel, ssize, 1, stdin);
    channel = ntohs(channel);

    // Transform & output samples if requested
    if (channel == 65535) {
      transform(dsamples, interval, 1);
      for(i=0; i<interval; i++) {
        ssamples[i] = htons((signed short)dsamples[i]);
      }
      fwrite(ssamples, ssize, interval, stdout);
      memset(dsamples, 0, dsize * interval);
      continue;
    }

    // Read sample into buffer
    fread(&usample, ssize, 1, stdin);
    usample = ntohs(usample);
    dsamples[channel] = hfloat_decode(usample);
  }
}
