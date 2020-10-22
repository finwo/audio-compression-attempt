#include <arpa/inet.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "helpers.h"

int main(int argc, char *argv[]) {
  argparse(argc, argv);

  // Calculate max channels per block
  double packetrate = (double)channels * inputrate / interval; // Packets per second
  double bps        = 16.0 * packetrate;                       // Packet overhead
  int maxchannels   = 0;
  int channelcount  = 0;
  while(bps < bitrate) {
    bps += 24.0 * packetrate;
    maxchannels++;
  }

  // Tracking for removing channels
  double highestAmplitude = 0;
  double currentAmplitude = 0;
  int highestIndex = 0;

  // Our own noise floor
  // Limited by hfloat
  noisefloor = 1.0 / 1024;

  fprintf(stderr, "Max: %d\n", maxchannels);

  fprintf(stderr, "Interval: %d\n", interval);
  int i=0;

  int ssize = sizeof(signed short);
  int dsize = sizeof(double);
  int bsize = ssize * interval;
  int rsize = 0;

  fprintf(stderr, "ssize: %d\n", ssize);
  fprintf(stderr, "bsize: %d\n", bsize);

  unsigned short channel;
  unsigned short usample;
  signed short *ssamples = malloc(ssize * interval);
  double *dsamples = malloc(dsize * interval);

  while(!feof(stdin)) {

    // Read block
    rsize = 0;
    while(rsize < interval) {
      rsize += fread(ssamples+rsize, ssize, interval - rsize, stdin);
    }

    // Convert to doubles
    for( i=0; i<interval; i++ ) {
      ssamples[i] = ntohs(ssamples[i]);
      dsamples[i] = (double)ssamples[i];
    }

    // Transform to pseudo-frequency-domain
    transform(dsamples, rsize, 2);

    // Output N loudest channels
    channelcount = 0;
    while(channelcount < maxchannels) {
      highestIndex     = 0;
      highestAmplitude = 0;
      for(i=0; i<interval; i++) {
        if (!dsamples[i]) continue;
        currentAmplitude = fabs(dsamples[i]);
        if (currentAmplitude <= highestAmplitude) continue;
        highestAmplitude = currentAmplitude;
        highestIndex     = i;
      }
      if (highestAmplitude < noisefloor) break;
      channel = htons((unsigned short)highestIndex);
      usample = hfloat_encode(dsamples[highestIndex]);
      usample = htons(usample);
      fwrite(&channel, ssize, 1, stdout);
      fwrite(&usample, ssize, 1, stdout);
      channelcount++;
      dsamples[highestIndex] = 0;
    }

    /* // Output only channels containing data */
    /* for( i=0; i<interval; i++ ) { */
    /*   if (!dsamples[i]) continue; */
    /*   channel = htons((unsigned short)i); */
    /*   usample = hfloat_encode(dsamples[i]); */
    /*   usample = htons(usample); */
    /*   fwrite(&channel, ssize, 1, stdout); */
    /*   fwrite(&usample, ssize, 1, stdout); */
    /* } */

    // Output delimiter
    channel = htons((unsigned short)65535);
    fwrite(&channel, ssize, 1, stdout);
  }

  return 0;
}
