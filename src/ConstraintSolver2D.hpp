#ifndef agi3d_constraintsolver2d_h
#define agi3d_constraintsolver2d_h

struct ConstSolve2D {
  static const float defaultInit[];
};

float * solver2D(float *, float *, float, float *);

#endif
