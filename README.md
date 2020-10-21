audio resample test
-------------------

## Rough idea

- Encoding/decoding explanation
  - Separate audio into multiple bands by quadrature mixing with multiple oscillators
  - Average mixes over time slots to spend only a single quadrature sample each timeslot
  - Upsample (bezier-based) quadrature samples for decoded slot to smooth frequencies and remove jitter
  - Using 80 Hz time slots, 255 frequency bands result in a 254*80=20320 Hz highest oscillator
- The compression
  - Remove silent frequency bands
  - If lower bitrate is required, remove lowest amplitude bands

## Container format

Muxes all channels

TODO: fully describe container once channels are stable

## Channel format

Channel data has a structure designed for simple encoding and decoding

```
<8 bits>
  00..FE  Frequency band indicator (+4 bytes)
            The frequency for the given band is timeslot rate times channel number
  FF      Time slot marker
[4 bytes]
  2x 16-bit big-endian sample
    1st: original samples multiplied with sin(f)
    2nd: original samples multiplied with cos(f)
```

## Encoding

- Read timeslot of samples
- For each oscillator
  - Multiply all samples by phase at sample
  - Sum all results together
  - output nnsssscccc (if either is !0)
- output FF

## Bitrate limiting

- Load whole timeslot
- Remove lowest-amplitude channel until you're happy
- Output timeslot again

## Decoding

**TOOD**: short explanation

<!--

decoding:
  foreach osc
    foreach sample in timeslot
      cubic smooth between timeslot
      calculate phase
      cubic smooth Q,I,~Q,~I -> sample

min bitrate =       5 * 8 * 80 =   3200 bps =   3.13 kbps
max bitrate = 256 * 5 * 8 * 80 = 819200 bps = 800.00 kbps


se7f8 =
  sign     =  1 bit
  exponent =  5 bits
  fraction = 10 bits

  theoratical:
    235dB dynamic range
     66dB snr

js:
log = n => (n.toString().length + Math.log10(parseFloat("0." + n)));

-->


---

# Re-visit

The initial domain transform was based on multi-band direct-conversion
quadrature rf receivers. While functional, this requires a lot of computing
power.

Based on a more recent idea, the frequency-domain representation does not need
to be accurate, just reversible.

Here's a pseudocode idea for a reversible domain transform function.

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
