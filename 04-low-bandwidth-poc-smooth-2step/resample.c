#include <arpa/inet.h>
#include <stdio.h>

#include "common.h"
#include "smooth.h"

int main(int argc, char *argv[]) {
  argparse(argc, argv);

  fprintf(stderr, "input rate  : %d\n", inputrate);
  fprintf(stderr, "output rate : %d\n", outputrate);

  short overshoot = 0;

  // Sample storage
  signed short sshort = 0;
  double sample0 = 0;
  double sample1 = 0;
  double sample2 = 0;
  double sample3 = 0;
  double osample = 0;

  double step  = ((double)inputrate) / ((double)outputrate);
  double phase = 0 - step;

  // Resample
  while(overshoot < 3) {

    // Step
    phase += step;
    while (phase >= 1) {
      phase -= 1;

      // Read or overshoot
      if (feof(stdin)) {
        overshoot++;
        sshort = 0;
      } else {
        fread(&sshort, 2, 1, stdin);
        sshort = ntohs(sshort);
      }

      // Shift samples
      sample0 = sample1;
      sample1 = sample2;
      sample2 = sample3;
      sample3 = (double)sshort;
    }

    // Interpolate
    osample = smooth(
        sample0,
        sample1,
        sample2,
        sample3,
        phase
    );

    // Output
    sshort = (signed short)osample;
    sshort = htons(sshort);
    fwrite(&sshort, 2, 1, stdout);
  }

  return 0;
}
