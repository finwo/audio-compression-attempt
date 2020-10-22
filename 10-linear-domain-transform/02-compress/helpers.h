#include <math.h>
#include <stdlib.h>

void swap(double *pa, double *pb, size_t sz) {
  double *p1 = pa, *p2 = pb;
  double tmp;
  while (sz--) {
    tmp   = *p1;
    *p1++ = *p2;
    *p2++ = tmp;
  }
}

void interleave(double *s, size_t len) {
  size_t start, step, i, j;

  if (len <= 2) {
    return;
  }

  if (len & (len - 1)) {
    return; // only power of 2 lengths are supported
  }

  for (start = 1, step = 2;
       step < len;
       start *= 2, step *= 2)
  {
    for (i = start, j = len / 2;
         i < len / 2;
         i += step, j += step)
    {
      swap(s + i,
           s + j,
           step / 2);
    }
  }
}

void transform(double *samples, int length, int divider) {
  if (length & (length -1)) return; // Only powers of 2 supported
  if (length <= 1) return;          // 1 = no change
  int hlength = length/2;
  double tmp;
  transform(samples, hlength, divider);
  transform(samples + hlength, hlength, divider);
  interleave(samples, length);
  for(int i = 0; i<length; i+=2) {
    tmp          = (samples[i+0] + samples[i+1]) / divider;
    samples[i+1] = (samples[i+0] - samples[i+1]) / divider;
    samples[i+0] = tmp;
  }
}

// Custom half-precision float
// SEEEEEFFFFFFFFFF
unsigned short hfloat_encode(double subject) {
  unsigned short sign     = 0;
  unsigned short exponent = 0;
  unsigned short fraction = 0;

  // Handle sign
  if (subject < 0) {
    sign    = 32768;
    subject = fabs(subject);
  }

  // Exponent
  while(subject >= 1) {
    exponent += 1024;
    subject  /= 2;
  }

  // Fraction
  fraction = (unsigned short)(subject * 1024);

  // Mix everything together
  return sign | exponent | fraction;
}

// Custom half-precision float
// SEEEEEFFFFFFFFFF
double hfloat_decode(unsigned short subject) {
  unsigned short fraction = subject & 0b0000001111111111;
  unsigned short exponent = subject & 0b0111110000000000;
  unsigned short sign     = subject & 0b1000000000000000;

  double result = ((double)fraction) / 1024;

  while(exponent) {
    result   *= 2;
    exponent -= 1024;
  }

  if (sign) {
    result = 0 - result;
  }

  return result;
}
