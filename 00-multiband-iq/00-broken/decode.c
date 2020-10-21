#include <arpa/inet.h>
#include <stdio.h>

#include "common.h"
#include "smooth.h"

int main(int argc, char *argv[]) {
  argparse(argc, argv);

  double phase = 0;
  double half = M_PI / ((double)interval);
  double cycle = half * 2;

  unsigned char cchannel;
  signed short pchannel;

  signed short sshort, cshort, ssample;
  int i,c,shortsize = sizeof(sshort);
  double dsample;
  double S,C;
  double I,Q;

  double SC[2048];
  for(i=0;i<2048;i++) SC[i] = 0;

  fprintf(stderr, "dec interval: %d\n", interval);

  double phases[256];
  for(i=0;i<256;i++) {
    phases[i] = 0;
  }

  while(!feof(stdin)) {
    fread(&cchannel, 1, 1, stdin);

    // If a new time slot arrives
    // 'print' the previous
    if (cchannel <= pchannel) {

      // All samples
      for(i=0; i<interval; i++) {
        dsample = 0;

        // For all oscillator channels
        for(c=0;c<256;c++) {

          // Smooth IQ samples
          I=smooth(SC[(c*8)+0],SC[(c*8)+2],SC[(c*8)+4],SC[(c*8)+6],
            ((double)i)/((double)interval)
          );
          Q=smooth(SC[(c*8)+1],SC[(c*8)+3],SC[(c*8)+5],SC[(c*8)+7],
            ((double)i)/((double)interval)
          );

          // Mix SIN * Imaginary + COS * Q
          sincos(phases[c], &S, &C);
          dsample += (S*I)+(C*Q);

          // And increment time for this oscillator
          phases[c] = fmod(phases[c] + (cycle*c), M_PI * 2);;
        }

        ssample = htons((signed short)dsample);
        fwrite(&ssample, shortsize, 1, stdout);
      }

    }

    // Read new IQ sample
    fread(&sshort, shortsize, 1, stdin);
    fread(&cshort, shortsize, 1, stdin);
    sshort = ntohs(sshort);
    cshort = ntohs(cshort);
    SC[(cchannel*8)+0] = SC[(cchannel*8)+2];
    SC[(cchannel*8)+1] = SC[(cchannel*8)+3];
    SC[(cchannel*8)+2] = SC[(cchannel*8)+4];
    SC[(cchannel*8)+3] = SC[(cchannel*8)+5];
    SC[(cchannel*8)+4] = SC[(cchannel*8)+6];
    SC[(cchannel*8)+5] = SC[(cchannel*8)+7];
    SC[(cchannel*8)+6] = sshort;
    SC[(cchannel*8)+7] = cshort;

    pchannel = cchannel;
  }

  return 0;
}
