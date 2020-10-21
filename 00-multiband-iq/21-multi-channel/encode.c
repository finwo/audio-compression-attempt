#include <arpa/inet.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

int main(int argc, char *argv[]) {
  argparse(argc, argv);

  double half  = M_PI / (double)interval;
  double cycle = half * 2;
  unsigned char c = 0;

  // Precalculate mixing bands
  double sinmask[255][interval];
  double cosmask[255][interval];
  int i = 0;
  for(i=0; i<interval; i++) {
    for( c=0; c<255; c++ ) {
      sinmask[c][i] = sin(cycle*c*i);
      cosmask[c][i] = cos(cycle*c*i);
    }
  }

  int ssize = sizeof(signed short);
  signed short ssample;

  double sc[255][4];
  for( c=0 ; c<255 ; c++ ) {
    sc[c][0] = 0;
    sc[c][1] = 0;
    sc[c][2] = 0;
    sc[c][3] = 0;
  }

  int hinterval = interval / 2;

  unsigned char channel;
  signed short sshort = 0;
  signed short cshort = 0;

  while(!feof(stdin)) {

    if (!i) {
      for( c=0; c<255; c++ ) {
        sshort = sc[c][0] / interval;
        cshort = sc[c][1] / interval;
        sshort = htons(sshort);
        cshort = htons(cshort);
        if (sshort | cshort) {
          fwrite(&c, 1, 1, stdout);
          fwrite(&sshort, ssize, 1, stdout);
          fwrite(&cshort, ssize, 1, stdout);
        }
        sc[c][0] = sc[c][2];
        sc[c][1] = sc[c][3];
        sc[c][2] = 0;
        sc[c][3] = 0;
      }
      channel = 255;
      fwrite(&channel, 1, 1, stdout);
    }

    fread(&ssample, ssize, 1, stdin);
    ssample = ntohs(ssample);

    for( c=0 ; c<255 ; c++ ) {
      sc[c][0] += ((double)ssample) * sinmask[c][i + hinterval];
      sc[c][1] += ((double)ssample) * cosmask[c][i + hinterval];
      sc[c][2] += ((double)ssample) * sinmask[c][i];
      sc[c][3] += ((double)ssample) * cosmask[c][i];
    }

    i = (i+1) % hinterval;
  }

  return 0;
}
