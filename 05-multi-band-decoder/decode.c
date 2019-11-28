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

  /* int dsize = sizeof(double); */
  int ssize = sizeof(signed short);

  unsigned char channel;

  signed short ssample;
  signed short sshort = 0;
  signed short cshort = 0;

  double dsample = 0;
  double S,C;
  double Q,I;

  double phases[256];
  for(i=0;i<256;i++) {
    phases[i] = 0;
  }

  double sc[2048];
  for(i=0;i<2048;i++) {
    sc[i] = 0;
  }

  while(!feof(stdin)) {
    fread(&channel, 1, 1, stdin);

    if (channel == 255) {
      for(i=0;i<hinterval;i++) {
        dsample = 0;

        for(c=0;c<255;c++) {
          sincos(phases[c], &S, &C);

          Q = smooth(sc[(c*8)+0],sc[(c*8)+2],sc[(c*8)+4],sc[(c*8)+6],((double)i) / ((double)hinterval));
          I = smooth(sc[(c*8)+1],sc[(c*8)+3],sc[(c*8)+5],sc[(c*8)+7],((double)i) / ((double)hinterval));

          dsample += (Q*S) + (I*C);

          phases[c] = fmod(phases[c] + (cycle * c), M_PI * 2);
        }

        ssample = dsample;
        ssample = htons(ssample);

        fwrite(&ssample, ssize, 1, stdout);
      }

      for(c=0;c<255;c++) {
        sc[(c*8)+0] = sc[(c*8)+2];
        sc[(c*8)+1] = sc[(c*8)+3];
        sc[(c*8)+2] = sc[(c*8)+4];
        sc[(c*8)+3] = sc[(c*8)+5];
        sc[(c*8)+4] = sc[(c*8)+6];
        sc[(c*8)+5] = sc[(c*8)+7];
        sc[(c*8)+6] = 0;
        sc[(c*8)+7] = 0;
      }

      continue;
    }

    fread(&sshort, ssize, 1, stdin);
    fread(&cshort, ssize, 1, stdin);
    sshort = ntohs(sshort);
    cshort = ntohs(cshort);

    c = channel;
    sc[(c*8)+6] = sshort;
    sc[(c*8)+7] = sshort;
  }

  return 0;
}
