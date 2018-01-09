#include "SimplexNoise.hpp"

int SimplexNoise::Integer(const double & x) const
{
  if (x<0) {
    return (int)(x) -1;
  }
  return (int)(x);
}

double dot(const int* g, const double& x, const double& y)
{
	return g[0] * x + g[1] * y;
}

double SimplexNoise::At(const Math::Vec2d& p) const
{
  const double& x = p[0];
  const double& y = p[1];
  // Noise contributions from the three corners
  double n[3] = { 0.0, 0.0, 0.0 };
  // Skew the input space to determine which simplex cell we are in
  const double F2 = 0.5 * (sqrt(3.0) - 1.0);
  // Hairy factor for 2D
  double s = (x + y) * F2;
  int i = Integer(x + s);
  int j = Integer(y + s);
  const double G2 = (3.0 - sqrt(3.0)) / 6.0;
  double t = (i + j) * G2;
  // Unskew the cell origin back to (x,y) space
  double X0 = i - t;
  double Y0 = j - t;
  // The x,y distances from the cell origin
  double x0 = x - X0;
  double y0 = y - Y0;
  // For the 2D case, the simplex shape is an equilateral triangle.
  // Determine which simplex we are in.
  int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
  if (x0 > y0) { i1 = 1; j1 = 0; } // lower triangle, XY order: (0,0)->(1,0)->(1,1)
  else { i1 = 0; j1 = 1; } // upper triangle, YX order: (0,0)->(0,1)->(1,1)
  // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
  // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where c = (3-sqrt(3))/6
  double x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
  double y1 = y0 - j1 + G2;
  double x2 = x0 - 1.0 + 2.0 * G2; // Offsets for last corner in (x,y) unskewed coords
  double y2 = y0 - 1.0 + 2.0 * G2;
  // Work out the hashed gradient indices of the three simplex corners
  int ii = i & 255;
  int jj = j & 255;
  int gi0 = perm[ii + perm[jj]] % 8;
  int gi1 = perm[ii + i1 + perm[jj + j1]] % 8;
  int gi2 = perm[ii + 1 + perm[jj + 1]] % 8;
  // Calculate the contribution from the three corners
  double t0 = 0.5 - x0*x0 - y0*y0;
  if (t0 >= 0)
  {
    t0 *= t0;
    n[0] = t0 * t0 * dot(grad2[gi0], x0, y0); 
  }
  double t1 = 0.5 - x1*x1 - y1*y1;
  if (t1 >= 0)
  {
    t1 *= t1;
    n[1] = t1 * t1 * dot(grad2[gi1], x1, y1);
  }
  double t2 = 0.5 - x2*x2 - y2*y2;
  if (t2 >= 0)
  {
    t2 *= t2;
    n[2] = t2 * t2 * dot(grad2[gi2], x2, y2);
  }
  // Add contributions from each corner to get the final noise value.
  // The result is scaled to return values in the interval [-1,1].
  return 70.0 * (n[0] + n[1] + n[2]);
}
