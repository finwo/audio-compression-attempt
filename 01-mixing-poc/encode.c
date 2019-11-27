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

  /* int dsize = sizeof(double); */
  int ssize = sizeof(signed short);

  signed short ssample;
  signed short sshort = 0;
  signed short cshort = 0;

  double phase = 0;
  double S,C;

  while(!feof(stdin)) {
    fread(&ssample, ssize, 1, stdin);
    ssample = ntohs(ssample);

    sincos(phase,&S,&C);
    sshort = (signed short)(((double)ssample) * S);
    cshort = (signed short)(((double)ssample) * C);

    sshort = htons(sshort);
    cshort = htons(cshort);

    fwrite(&sshort, ssize, 1, stdout);
    fwrite(&cshort, ssize, 1, stdout);

    phase += cycle;
  }

  return 0;
}
