#include <math.h>
#include <stdio.h>
#include "math_nn.h"

double sigmoid(double x)
{
  return 1.0 / (1.0 + exp(-x));
}

double sigmoidDerivative(double x)
{
  return x * (1.0 - x);
}