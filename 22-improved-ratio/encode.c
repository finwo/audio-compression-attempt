#include <arpa/inet.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

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
      sinmask[c][i] = sin((cycle*c*i)+half);
      cosmask[c][i] = cos((cycle*c*i)+half);
    }
  }

  int ssize = sizeof(signed short);
  signed short ssample;

  double sc[255][2];
  for( c=0 ; c<255 ; c++ ) {
    sc[c][0] = 0;
    sc[c][1] = 0;
  }

  unsigned char channel;
  signed short sshort     = 0;
  signed short cshort     = 0;
  signed short loudest    = 0;
  signed short nf         = 0;

  while(!feof(stdin)) {

    if (!i) {

      // Fetch loudest channel
      loudest = 0;
      for( c=0 ; c<255 ; c++ ) {
        sshort = sc[c][0] / interval;
        cshort = sc[c][1] / interval;
        loudest = max(loudest, abs(sshort));
        loudest = max(loudest, abs(cshort));
      }

      nf = (signed short)((double)loudest * noisefloor);

      for( c=0; c<255; c++ ) {
        sshort = sc[c][0] / interval;
        cshort = sc[c][1] / interval;

        // Output if above noisefloor
        if ((abs(sshort)>nf) || (abs(cshort)>nf)) {
          sshort = htons(sshort);
          cshort = htons(cshort);
          fwrite(&c, 1, 1, stdout);
          fwrite(&sshort, ssize, 1, stdout);
          fwrite(&cshort, ssize, 1, stdout);
        }

        sc[c][0] = 0;
        sc[c][1] = 0;
      }
      channel = 255;
      fwrite(&channel, 1, 1, stdout);
    }

    fread(&ssample, ssize, 1, stdin);
    ssample = ntohs(ssample);

    for( c=0 ; c<255 ; c++ ) {
      sc[c][0] += ((double)ssample) * sinmask[c][i];
      sc[c][1] += ((double)ssample) * cosmask[c][i];
    }

    i = (i+1) % interval;
  }

  return 0;
}
