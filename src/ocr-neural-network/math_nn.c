#include "math_nn.h"

double sigmoid(double x)
{
  return 1.0 / (1.0 + exp(-x));
  // if (x > 0)
  //   return x;
  // else
  //   return 0;
}

double sigmoidDerivative(double x)
{
  // return x * (1.0 - x);
  if (x > 0)
    return 1;
  else
    return 0;
}

// implement softmax potentially