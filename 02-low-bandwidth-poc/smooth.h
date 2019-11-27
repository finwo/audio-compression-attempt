/* // catmull-rom spline */
/* //            sample     sample     sample     sample     0..1 */
/* double smooth (double y0, double y1, double y2, double y3, double mu) { */
/*   double a0,a1,a2,a3,mu2; */
/*   mu2 = mu*mu; */
/*   a0 = -0.5*y0 + 1.5*y1 - 1.5*y2 + 0.5*y3; */
/*   a1 = y0 - 2.5*y1 + 2*y2 - 0.5*y3; */
/*   a2 = -0.5*y0 + 0.5*y2; */
/*   a3 = y1; */
/*   return(a0*mu*mu2+a1*mu2+a2*mu+a3); */
/* } */

// custom bezier-based smoothing function
double smooth (double y0, double y1, double y2, double y3, double mu) {
  double imu  = 1-mu,
      x1 = y1 + (y2 - y0) / 6, // End of slope y1
      x2 = y2 + (y1 - y3) / 6, // End of slope y1
      a1 = y1*imu + x1*mu,     // Slide between y1 -> x1
      a2 = x1*imu + x2*mu,     // Slide between x1 -> x2
      a3 = x2*imu + y2*mu,     // Slide between x2 -> y2
      b1 = a1*imu + a2*mu,     // Slide between a1 -> a2
      b2 = a2*imu + a3*mu;     // Slide between a2 -> a3
  return b1*imu + b2*mu;       // Slide between b1 -> b2
}

/* // linear */
/* double smooth (double y0, double y1, double y2, double y3, double mu) { */
/*   double imu  = 1-mu; */
/*   return y1*imu + y2*mu;       // Slide between b1 -> b2 */
/* } */
