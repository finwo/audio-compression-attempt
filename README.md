audio resample test
-------------------

Rough idea:

- Encoding/decoding explanation
  - Separate audio into multiple bands by quadrature mixing with multiple oscillators
  - Average mixes over time slots to spend only a single quadrature sample each timeslot
  - Upsample (bezier-based) quadrature samples for decoded slot to smooth frequencies and remove jitter
- The compression
  - Remove silent frequency bands
  - If lower bitrate is required, remove lowest amplitude bands
