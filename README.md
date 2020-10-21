# Audio resample test

The initial domain transform was based on multi-band direct-conversion
quadrature rf receivers. While functional, this requires a lot of computing
power.

Based on a more recent idea, the frequency-domain representation does not need
to be accurate, just reversible.

## Rough idea

Pseudocode idea for a reversible domain transform function:

```
fn(arr) {
  left = fn(arr/2)
  right = fn(arr/2)
  for i in arr/2
    arr[i*2  ] = (left[i] + right[i]) / 2
    arr[i*2+1] = (left[i] - right[i]) / 2
  return arr
}
```
