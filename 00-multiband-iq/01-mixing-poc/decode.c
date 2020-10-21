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
    fread(&sshort, ssize, 1, stdin);
    fread(&cshort, ssize, 1, stdin);
    sshort = ntohs(sshort);
    cshort = ntohs(cshort);

    sincos(phase,&S,&C);
    ssample = (signed short)(
        (((double)sshort) * S) +
        (((double)cshort) * C)
    );
    ssample = htons(ssample);

    fwrite(&ssample, ssize, 1, stdout);

    phase += cycle;
  }

  return 0;
}
