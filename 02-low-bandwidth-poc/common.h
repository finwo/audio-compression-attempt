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

int interval   = 1200;
int inputrate  = 44100;
int outputrate = 44100;

void argparse(int argc, char *argv[]) {
  int i;

  for(i=1;i<argc;i++) {

    if ((strcmp("-r"        , argv[i]) == 0) ||
        (strcmp("--interval", argv[i]) == 0)
    ) {
      i++;
      interval = atoi(argv[i]);
    }

    if ((strcmp("-i"          , argv[i]) == 0) ||
        (strcmp("--input-rate", argv[i]) == 0)
    ) {
      i++;
      inputrate = atoi(argv[i]);
    }

    if ((strcmp("-o"           , argv[i]) == 0) ||
        (strcmp("--output-rate", argv[i]) == 0)
    ) {
      i++;
      outputrate = atoi(argv[i]);
    }

  }

}

#ifdef __cplusplus
} // extern "C"
#endif
