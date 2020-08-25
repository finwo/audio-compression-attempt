#include <arpa/inet.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "dht.h"
#include "smooth.h"

#define MAX(a,b) ((a>b)?a:b)
#define MIN(a,b) ((a<b)?a:b)

double weight_band(int band) {
  int hinterval = interval / 2;
  if (band > hinterval) band = interval - band;
  double f  = inputrate / interval * band / channels;
  double F  = f;
  double LA = 0;
  double LF = 0;
  double FD = 0;
  int i;
  for(i=0; freqs[i+1]; i++) {
    if ((f < freqs[i]) || (f > freqs[i+1])) { LF=freqs[i]; continue; }
    F  = f - freqs[i];
    FD = freqs[i+1] - freqs[i];
    F  = F / FD;
    return pow(10, ((weighting[i] * (1-F)) + (weighting[i+1] * F)) / 20);
  }
  return 0.01; // -60dB, fallback (ultrasound, stereo, etc)
}

int main(int argc, char *argv[]) {
  argparse(argc, argv);
  int i=0;
  int c=0;
  int j=0;
  int skipped = 0;

  int hinterval = interval / 2;
  int dsize = sizeof(double);
  int ssize = sizeof(signed short);

  signed short ssample;
  double msample,osample,dsample;

  double *f = malloc(interval * dsize);
  double *o = malloc(interval * dsize);
  for(i=0; i<interval; i++) {
    *(f+i) = 0;
    *(o+i) = 0;
  }

  i=0;
  while(!feof(stdin)) {
    fread( &ssample, ssize, 1, stdin );
    ssample = ntohs(ssample);
    osample = ((double)ssample) / 32768;

    if (i == 0) {
      rec_fht_dit2(f, interval, o);

      // Detect peak
      msample = 0;
      for(c=0; c<interval; c++) {
        dsample = fabs(*(o+c)) * weight_band(c);
        if (dsample > msample) msample = dsample;
      }
      msample = msample * noisefloor;

      // Output values
      j = 0;
      skipped = 0;
      for(c=0; c<interval; c++) {
        dsample = *(o+c);
        if (fabs(dsample) < (1.0/32768.0)) {
          skipped++;
          continue;
        }
        if (fabs(dsample) * weight_band(c) <= msample) {
          skipped++;
          continue;
        }

        // Channel increment symbol
        putchar((c - j)>>8);
        putchar(c - j);
        j = c;

        // Write sample
        ssample = (int)(dsample / interval * 32768);
        ssample = htons(ssample);
        fwrite(&ssample, ssize, 1, stdout);
      }

      // Trigger output in the encoder
      putchar(255);

      // Shift HALF
      for(c=0; c<hinterval; c++) {
        *(f+c) = *(f+c+hinterval);
        *(f+c+hinterval) = 0;
      }

      /* fprintf(stderr, "Rate: %f\n", 100*(((double)interval-(double)skipped))/(double)hinterval); */
    }

    // Store the current sample
    *(f+i+hinterval) = osample;

    // Prepare next run's I
    i = (i+1) % hinterval;
  }

  return 0;
}
