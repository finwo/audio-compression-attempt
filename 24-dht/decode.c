#define _GNU_SOURCE

#include <arpa/inet.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "dht.h"

int main(int argc, char *argv[]) {
  argparse(argc, argv);
  int i=0;
  int c=0;

  int dsize = sizeof(double);
  int ssize = sizeof(signed short);

  signed short ssample;
  double dsample;
  int hinterval = interval / 2;

  unsigned char channel;

  double *f = malloc(interval * dsize);
  double *o = malloc(interval * dsize);
  double *s = malloc(interval * dsize);
  for(i=0; i<interval; i++) {
    *(f+i) = 0;
    *(o+i) = 0;
    *(s+i) = 0;
  }

  i = 0;
  while(!feof(stdin)) {
    fread(&channel, 1, 1, stdin);

    if (channel == 255) {
      rec_fht_dit2(f, interval, o);

      // Add faded to S buffer
      for(c=0; c<interval; c++) {
        dsample  = *(o+c);
        dsample *= (1 - cos(M_PI / hinterval * c)) / 2;
        *(s+c)  += dsample ;
      }

      // Output & shift half S buffer
      for(c=0; c<hinterval; c++) {
        dsample = *(s+c);
        ssample = (int)(dsample * 32768);
        ssample = htons(ssample);
        fwrite(&ssample, ssize, 1, stdout);
        *(s+c) = *(s+c+hinterval);
        *(s+c+hinterval) = 0;
      }

      // Reset f
      for(c=0;c<interval;c++) {
        *(f+c) = 0;
      }

      i = 0;
      continue;
    }

    // Find channel
    i += channel << 8;
    fread(&channel, 1, 1, stdin);
    i += channel;

    // Store sample
    fread(&ssample, ssize, 1, stdin);
    ssample = ntohs(ssample);
    dsample = ((double)ssample) / 32768;
    *(f+i) = dsample;
  }

  return 0;
}
