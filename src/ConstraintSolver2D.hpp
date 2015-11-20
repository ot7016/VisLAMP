#include <stdio.h>
#include <math.h>
#include <lbfgs.h>
#include <iostream>

#ifndef agi3d_constraintsolver2d_h
#define agi3d_constraintsolver2d_h

struct ConstSolve2D {
  static const double defaultInit[];
};

double * solver2D(double *, double *, double, double *);

#endif
