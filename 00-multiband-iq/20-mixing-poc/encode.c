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

  // Precalculate mixing bands
  double sinmask[interval];
  double cosmask[interval];
  int i = 0;
  for(i=0; i<interval; i++) {
    sincos(cycle*i, &sinmask[i], &cosmask[i]);
  }

  int ssize = sizeof(signed short);
  signed short ssample;

  double sc[4] = {0, 0, 0, 0};

  int hinterval = interval / 2;

  unsigned char channel;
  signed short sshort = 0;
  signed short cshort = 0;

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

    sc[0] += ((double)ssample) / interval * sinmask[i + hinterval];
    sc[1] += ((double)ssample) / interval * cosmask[i + hinterval];
    sc[2] += ((double)ssample) / interval * sinmask[i];
    sc[3] += ((double)ssample) / interval * cosmask[i];

    i = (i+1) % hinterval;
  }

  return 0;
}
