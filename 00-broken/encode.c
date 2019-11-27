#include <arpa/inet.h>
#include <stdio.h>

#include "common.h"

int main(int argc, char *argv[]) {
  argparse(argc, argv);

  double half  = M_PI / (double)interval;
  double cycle = half * 2;

  signed short ssample;
  int shortsize = sizeof(ssample);
  unsigned short channel = 0;
  int i,c;

  signed short sshort = 0;
  signed short cshort = 0;

  double S,C;
  double sc[512];

  double phases[256];
  for(i=0;i<256;i++) {
    phases[i] = 0;
  }

  double phase = 0;
  fprintf(stderr, "enc interval: %d\n", interval);

  while(!feof(stdin)) {
    for(i=0;i<512;i++) sc[i] = 0;

    for(i=0;i<interval;i++) {

      // Read sample
      fread(&ssample, shortsize, 1, stdin);
      ssample = ntohs(ssample);

      for(c=0;c<16;c++) {
        sincos(phases[c], &S, &C);
        sc[(c*2)+0] += ssample * S / interval;
        sc[(c*2)+1] += ssample * C / interval;
        phases[c] = fmod(phases[c] + (cycle*c), M_PI * 2);;
      }

    }

    for( c=1; c<2; c++ ) {
      channel = c;
      fwrite(&channel, 1, 1, stdout);
      sshort = htons((signed short)(sc[16]));
      cshort = htons((signed short)(sc[17]));
      fwrite(&sshort, shortsize, 1, stdout);
      fwrite(&cshort, shortsize, 1, stdout);
    }
  }

  return 0;
}
