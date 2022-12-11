#include <math.h>
#include <stdio.h>

#ifndef MATH_NN_H
#  define MATH_NN_H

/// @brief Apply the sigmoid function to a value.
/// @param x The value to apply the sigmoid function to.
/// @return The result of the sigmoid function.
double sigmoid(double x);

/// @brief Apply the derivative of the sigmoid function to a value.
/// @param x The value to apply the derivative of the sigmoid function to.
/// @return The result of the derivative of the sigmoid function.
double sigmoidDerivative(double x);

#endif