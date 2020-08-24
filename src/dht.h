#include <math.h>
#include <stdlib.h>

void hartley_shift(double c[], int n) {
  double S,C,t;
  int nh  = n/2;
  int k,j = n-1;
  for (k=1;k<nh;k++) {
    sincos( M_PI * k / n, &S, &C );
    t    = c[k] * C + c[j] * S;
    c[j] = c[k] * S - c[j] * C;
    c[k] = t;
    j = j - 1;
  }
}

void rec_fht_dit2(double a[], int n, double x[]) {
  int k, nh = n / 2;
  double as = n * sizeof(double);
  double *b = malloc(as);
  double *c = malloc(as);
  double *s = malloc(as);
  double *t = malloc(as);
  if (n == 1) {
    x[0] = a[0];
    free(b);
    free(c);
    free(s);
    free(t);
    return;
  }
  for (k=0;k<nh;k++) {
    s[k] = a[2*k];   // even indexed elements
    t[k] = a[2*k+1]; // odd indexed elements
  }
  rec_fht_dit2(s, nh, b);
  rec_fht_dit2(t, nh, c);
  hartley_shift(c, nh);
  for (k=0;k<nh;k++) {
    x[k]    = b[k] + c[k];
    x[k+nh] = b[k] - c[k];
  }

  free(b);
  free(c);
  free(s);
  free(t);
}
