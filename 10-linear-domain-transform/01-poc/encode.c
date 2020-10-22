#include <arpa/inet.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "helpers.h"

int main(int argc, char *argv[]) {
  argparse(argc, argv);

  fprintf(stderr, "Interval: %d\n", interval);
  int i=0;

  int ssize = sizeof(signed short);
  int dsize = sizeof(double);
  int bsize = ssize * interval;
  int rsize = 0;

  fprintf(stderr, "ssize: %d\n", ssize);
  fprintf(stderr, "bsize: %d\n", bsize);

  unsigned short channel;
  unsigned short usample;
  signed short *ssamples = malloc(ssize * interval);
  double *dsamples = malloc(dsize * interval);

  while(!feof(stdin)) {

    // Read block
    rsize = 0;
    while(rsize < interval) {
      rsize += fread(ssamples+rsize, ssize, interval - rsize, stdin);
    }

    // Convert to doubles
    for( i=0; i<interval; i++ ) {
      ssamples[i] = ntohs(ssamples[i]);
      dsamples[i] = (double)ssamples[i];
    }

    // Transform to pseudo-frequency-domain
    transform(dsamples, rsize, 2);

    // Output only channels containing data
    for( i=0; i<interval; i++ ) {
      /* if (fabs(dsamples[i])<(1.0/1024)) continue; */
      channel = htons((unsigned short)i);
      usample = hfloat_encode(dsamples[i]);
      usample = htons(usample);
      fwrite(&channel, ssize, 1, stdout);
      fwrite(&usample, ssize, 1, stdout);
    }

    // Output delimiter
    channel = htons((unsigned short)65535);
    fwrite(&channel, ssize, 1, stdout);
  }

  return 0;
}
