#include <arpa/inet.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "smooth.h"

int main(int argc, char *argv[]) {
  argparse(argc, argv);

  double half  = M_PI / (double)interval;
  double cycle = half * 2;

  int c=0, i=0;
  int hinterval = interval / 2;

  // Precalculate mixing bands
  double sinmask[interval];
  double cosmask[interval];
  for(i=0; i<interval; i++) {
    sincos(cycle*i, &sinmask[i], &cosmask[i]);
    sinmask[i] *= 1 - cos(M_PI * 2 * i / interval);
    cosmask[i] *= 1 - cos(M_PI * 2 * i / interval);
  }

  int ssize = sizeof(signed short);
  unsigned char channel;

  signed short ssample;
  signed short sshort = 0;
  signed short cshort = 0;

  double ssamples[2][hinterval];
  for(i=0; i<hinterval; i++) {
    ssamples[0][i] = 0;
    ssamples[1][i] = 0;
  }

  while(!feof(stdin)) {
    fread(&channel, 1, 1, stdin);

    if (channel == 255) {
      for( i=0; i<hinterval; i++ ) {
        ssample = ssamples[0][i];
        ssample = htons(ssample);
        fwrite(&ssample, ssize, 1, stdout);
        ssamples[0][i] = ssamples[1][i];
        ssamples[1][i] = 0;
      }

      continue;
    }

    fread(&sshort, ssize, 1, stdin);
    fread(&cshort, ssize, 1, stdin);
    sshort = ntohs(sshort);
    cshort = ntohs(cshort);

    for( i=0; i<hinterval; i++ ) {
      ssamples[0][i] += sinmask[i+hinterval] * sshort;
      ssamples[0][i] += cosmask[i+hinterval] * cshort;
      ssamples[1][i] += sinmask[i] * sshort;
      ssamples[1][i] += cosmask[i] * cshort;
    }

  }

  return 0;
}
