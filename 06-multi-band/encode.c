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

  int hinterval = interval / 2;

  int c=0, i=0;

  /* int dsize = sizeof(double); */
  int ssize = sizeof(signed short);

  signed short ssample;
  unsigned char channel;
  signed short sshort = 0;
  signed short cshort = 0;

  /* double dsample = 0; */
  double S,C;

  double phases[256];
  for(i=0;i<256;i++) {
    phases[i] = 0;
  }

  double sc[1024];
  for(i=0;i<1024;i++) {
    sc[i] = 0;
  }

  while(!feof(stdin)) {
    fread( &ssample, ssize, 1, stdin );
    ssample = ntohs(ssample);

    if (i == 0) {
      for(c=0;c<255;c++) {
        channel = c;
        sshort = sc[(c*4)+0];
        cshort = sc[(c*4)+0];

        sc[(c*4)+0] = sc[(c*4)+2];
        sc[(c*4)+1] = sc[(c*4)+3];
        sc[(c*4)+2] = 0;
        sc[(c*4)+3] = 0;

        if (!(sshort|cshort)) {
          continue;
        }

        sshort = htons(sshort);
        cshort = htons(cshort);
        fwrite(&channel, 1, 1, stdout);
        fwrite(&sshort, ssize, 1, stdout);
        fwrite(&cshort, ssize, 1, stdout);
      }
      channel = 255;
      fwrite(&channel, 1, 1, stdout);
    }

    for(c=0;c<255;c++) {
      sincos(phases[c], &S, &C);
      sc[(c*4)+0] += S * ssample / interval;
      sc[(c*4)+2] += S * ssample / interval;
      sc[(c*4)+1] += C * ssample / interval;
      sc[(c*4)+3] += C * ssample / interval;
      phases[c] = fmod(phases[c] + (cycle * c), M_PI * 2);
    }

    i = (i+1) % hinterval;
  }

  return 0;
}
