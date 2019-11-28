/* // cubic interpolation */
/* double smooth( */
/*    double y0,double y1, */
/*    double y2,double y3, */
/*    double mu) */
/* { */
/*    double a0,a1,a2,a3,mu2; */
/*    mu2 = mu*mu; */
/*    a0 = y3 - y2 - y0 + y1; */
/*    a1 = y0 - y1 - a0; */
/*    a2 = y2 - y0; */
/*    a3 = y1; */
/*    return(a0*mu*mu2+a1*mu2+a2*mu+a3); */
/* } */

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

/* // Tension: 1 is high, 0 normal, -1 is low */
/* // Bias: 0 is even, */
/* //       positive is towards first segment, */
/* //       negative towards the other */
/* double smooth( */
/*    double y0,double y1, */
/*    double y2,double y3, */
/*    double mu */
/*    /1* , double tension *1/ */
/*    /1* , double bias *1/ */
/* ) { */
/*   double m0,m1,mu2,mu3; */
/*   double a0,a1,a2,a3; */
/*   double tension = 0; */
/*   double bias    = 0; */
/*   mu2 = mu * mu; */
/*   mu3 = mu2 * mu; */
/*   m0  = (y1-y0)*(1+bias)*(1-tension)/2; */
/*   m0 += (y2-y1)*(1-bias)*(1-tension)/2; */
/*   m1  = (y2-y1)*(1+bias)*(1-tension)/2; */
/*   m1 += (y3-y2)*(1-bias)*(1-tension)/2; */
/*   a0 =  2*mu3 - 3*mu2 + 1; */
/*   a1 =    mu3 - 2*mu2 + mu; */
/*   a2 =    mu3 -   mu2; */
/*   a3 = -2*mu3 + 3*mu2; */
/*   return(a0*y1+a1*m0+a2*m1+a3*y2); */
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
