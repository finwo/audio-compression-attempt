#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899L
#endif

#ifndef M_2PI
#define M_2PI (2*M_PI)
#endif

int interval      = 1200;
int inputrate     = 44100;
int outputrate    = 44100;
int channels      = 1;
double noisefloor = 0;

double freqs[] = {
  8,
  16,
  31.5,
  63,
  125,
  250,
  500,
  1000,
  2000,
  4000,
  8000,
  16000,
  20000,
  0,
};
double weightA[] = {
  -77.5, // 8
  -56.4, // 16
  -39.5, // 31.5
  -26.2, // 63
  -16.2, // 125
  -8.7, // 250
  -3.2, // 500
  0, // 1000
  1.2, // 2000
  1.0, // 4000
  -1.1, // 8000
  -6.7, // 16000
  -9.3, // 20000
};
double weightC[] = {
  -17.6, // 8
  -8.4, // 16
  -3.0, // 31.5
  -0.8, // 63
  -0.2, // 125
  0, // 250
  0, // 500
  0, // 1000
  -0.2, // 2000
  -0.8, // 4000
  -3.0, // 8000
  -8.6, // 16000
  -11.2, // 20000
};
double *weighting = NULL;

void argparse(int argc, char *argv[]) {
  int i, nf = -12;

  for(i=1;i<argc;i++) {

    if ((strcmp("-r"        , argv[i]) == 0) ||
        (strcmp("--interval", argv[i]) == 0)
    ) {
      i++;
      interval = atoi(argv[i]);
      continue;
    }

    if ((strcmp("-i"          , argv[i]) == 0) ||
        (strcmp("--input-rate", argv[i]) == 0)
    ) {
      i++;
      inputrate = atoi(argv[i]);
      continue;
    }

    if ((strcmp("-o"           , argv[i]) == 0) ||
        (strcmp("--output-rate", argv[i]) == 0)
    ) {
      i++;
      outputrate = atoi(argv[i]);
      continue;
    }

    if ((strcmp("-n"          , argv[i]) == 0) ||
        (strcmp("--noisefloor", argv[i]) == 0)
    ) {
      i++;
      nf = 0-atoi(argv[i]);
      continue;
    }

    if ((strcmp("-c"        , argv[i]) == 0) ||
        (strcmp("--channels", argv[i]) == 0)
    ) {
      i++;
      channels = atoi(argv[i]);
      continue;
    }

    if ((strcmp("-A"        , argv[i]) == 0) ||
        (strcmp("--dbA", argv[i]) == 0)
    ) {
      weighting = weightA;
      continue;
    }

    if ((strcmp("-C"        , argv[i]) == 0) ||
        (strcmp("--dbC", argv[i]) == 0)
    ) {
      weighting = weightC;
      continue;
    }

  }

  noisefloor = pow(10, (double)nf/20);

  // Z-weighting fallback
  if (!weighting) {
    weighting = calloc(13,sizeof(double));
  }
}

#ifdef __cplusplus
} // extern "C"
#endif
