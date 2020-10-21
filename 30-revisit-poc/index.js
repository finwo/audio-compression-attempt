const cnt     = 2**6;
const samples = Array(cnt).fill(0).map((a,i) => Math.sin(i * 4 * Math.PI / cnt));

const fn = (arr,div=2) => {
  if (arr.length <= 1) return arr;
  const left = fn(arr.slice(0,arr.length/2),div);
  const right = fn(arr.slice(arr.length/2),div);
  for(const i in left) {
    arr[i*2  ] = (left[i] + right[i]) / div;
    arr[i*2+1] = (left[i] - right[i]) / div;
  }
  return arr;
};

// Show original samples
console.log('');
for(let sample of samples) {
  sample = 25 + (sample * 20)
  console.log('='.repeat(Math.floor(sample)));
}

// Run conversion
fn(samples);
console.log('');
for(let sample of samples) {
  sample = 25 + (sample * 20)
  console.log('='.repeat(Math.floor(sample)));
}

// Reconstruct
console.log('');
fn(samples,1);
for(let sample of samples) {
  sample = 25 + (sample * 20)
  console.log('='.repeat(Math.floor(sample)));
}

