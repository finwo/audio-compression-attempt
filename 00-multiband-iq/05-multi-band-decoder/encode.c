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

  int i = 0;

  /* int dsize = sizeof(double); */
  int ssize = sizeof(signed short);
  unsigned char channel = 0;

  signed short ssample;
  signed short sshort = 0;
  signed short cshort = 0;

  double dsample = 0;
  double phase = 0;
  double S,C;

  double sc[4];
  sc[0] = 0;
  sc[1] = 0;
  sc[2] = 0;
  sc[3] = 0;

  while(!feof(stdin)) {
    fread( &ssample, ssize, 1, stdin );
    ssample = ntohs(ssample);

    if (i == 0) {
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

    sincos(phase, &S, &C);
    sc[0] += S * ssample / interval;
    sc[2] += S * ssample / interval;
    sc[1] += C * ssample / interval;
    sc[3] += C * ssample / interval;

    i = (i+1) % hinterval;
    phase = fmod(phase + cycle, M_PI * 2);
  }

  return 0;
}
