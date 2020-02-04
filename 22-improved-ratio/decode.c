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
  double sinmask[255][interval];
  double cosmask[255][interval];
  for(i=0; i<interval; i++) {
    for( c=0 ; c<255 ; c++ ) {
      sinmask[c][i] = sin((cycle*c*i)+half);
      cosmask[c][i] = cos((cycle*c*i)+half);
    }
  }

  int ssize = sizeof(signed short);
  unsigned char channel;

  signed short ssample;
  signed short sshort = 0;
  signed short cshort = 0;

  double ssamples[2][interval];
  for(i=0; i<interval; i++) {
    ssamples[0][i] = 0;
    ssamples[1][i] = 0;
  }

  while(!feof(stdin)) {
    fread(&channel, 1, 1, stdin);

    if (channel == 255) {
      for( i=0; i<interval; i++ ) {
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

    for( i=0; i<interval; i++ ) {
      ssamples[0][i] += sinmask[channel][i] * sshort;
      ssamples[0][i] += cosmask[channel][i] * cshort;
    }

  }

  return 0;
}
