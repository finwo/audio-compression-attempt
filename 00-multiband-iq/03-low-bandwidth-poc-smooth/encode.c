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

  int i = 0;

  /* int dsize = sizeof(double); */
  int ssize = sizeof(signed short);

  signed short ssample;
  signed short sshort = 0;
  signed short cshort = 0;

  double phase = 0;
  double S,C;

  double sc[2];

  while(!feof(stdin)) {
    sc[0] = 0;
    sc[1] = 0;

    for(i=0; i<interval; i++) {
      fread(&ssample, ssize, 1, stdin);
      ssample = ntohs(ssample);

      sincos(phase,&S,&C);
      sc[0] += ((double)ssample) * S / interval;
      sc[1] += ((double)ssample) * C / interval;

      phase += cycle;
    }

    sshort = htons((signed short)(sc[0]));
    cshort = htons((signed short)(sc[1]));

    fwrite(&sshort, ssize, 1, stdout);
    fwrite(&cshort, ssize, 1, stdout);
  }

  return 0;
}
