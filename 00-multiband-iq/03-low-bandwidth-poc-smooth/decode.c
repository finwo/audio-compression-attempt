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

  int i = 0;

  /* int dsize = sizeof(double); */
  int ssize = sizeof(signed short);

  signed short ssample;
  signed short sshort = 0;
  signed short cshort = 0;

  double dsample = 0;
  double phase = 0;
  double S,C;
  double Q,I;

  double sc[8];
  for(i=0;i<8;i++) {
    sc[i] = 0;
  }

  while(!feof(stdin)) {
    fread(&sshort, ssize, 1, stdin);
    fread(&cshort, ssize, 1, stdin);
    sshort = ntohs(sshort);
    cshort = ntohs(cshort);
    sc[0] = sc[2];
    sc[1] = sc[3];
    sc[2] = sc[4];
    sc[3] = sc[5];
    sc[4] = sc[7];
    sc[5] = sc[3];
    sc[6] = (double)sshort;
    sc[7] = (double)cshort;

    for(i=0;i<interval;i++) {
      sincos(phase,&S,&C);

      Q = smooth(sc[0],sc[2],sc[4],sc[6],((double)i) / ((double)interval));
      I = smooth(sc[1],sc[3],sc[5],sc[7],((double)i) / ((double)interval));

      dsample = (Q*S) + (I*C);
      ssample = dsample;
      ssample = htons(ssample);

      fwrite(&ssample, ssize, 1, stdout);

      phase += cycle;
    }
  }

  return 0;
}
