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

  int ssize = sizeof(signed short);
  signed short ssample;

  int i = 0;

  int hinterval = interval / 2;

  unsigned char channel;
  signed short sshort = 0;
  signed short cshort = 0;

  double phase = 0;
  double S,C;

  double sc[4];
  for( i=0; i<4; i++ ) sc[i] = 0;

  while(!feof(stdin)) {

    if (!i) {
      sshort = sc[0];
      cshort = sc[1];
      sshort = htons(sshort);
      cshort = htons(cshort);
      channel = 1;
      fwrite(&channel, 1, 1, stdout);
      fwrite(&sshort, ssize, 1, stdout);
      fwrite(&cshort, ssize, 1, stdout);
      channel = 255;
      fwrite(&channel, 1, 1, stdout);
      sc[0] = sc[2];
      sc[1] = sc[3];
      sc[2] = 0;
      sc[3] = 0;
    }

    fread(&ssample, ssize, 1, stdin);
    ssample = ntohs(ssample);

    sincos(phase, &S, &C);

    sc[0] += ((double)ssample) * S / interval * (1 + cos( M_PI * ((double)i) / ((double)hinterval) ));
    sc[1] += ((double)ssample) * C / interval * (1 + cos( M_PI * ((double)i) / ((double)hinterval) ));
    sc[2] += ((double)ssample) * S / interval * (1 - cos( M_PI * ((double)i) / ((double)hinterval) ));
    sc[3] += ((double)ssample) * C / interval * (1 - cos( M_PI * ((double)i) / ((double)hinterval) ));

    i = (i+1) % hinterval;
    phase += cycle;
  }

  return 0;
}
