#include <stdio.h>
#include <stdlib.h>
#include "solver/main.h"
#include "image-processing/main.h"
#include "image-processing/utils/splitting.h"
#include "xor-neural-network/main.h"

int main(int argc, char **argv)
{
  if (argc >= 2)
  {
    // make a switch
    if (strcmp(argv[1], "image-processing") == 0)
    {
      imageProcessingMain(argc - 1, argv + 1);
    }
    else if (strcmp(argv[1], "solver") == 0)
    {
      solverMain(argc - 1, argv + 1);
    }
    else if (strcmp(argv[1], "image-rotate") == 0)
    {
      imageRotateMain(argc - 1, argv + 1);
    }
    else if (strcmp(argv[1], "xor-neural-network") == 0)
    {
      xorNeuralNetworkMain(argc - 1, argv + 1);
    }
    else if (strcmp(argv[1], "image-splitting") == 0)
    {
      splitMain(argc - 1, argv + 1);
    }
    else
    {
      errx(1, "usage: %s solver or image-processing ...", argv[0]);
    }
  }
  else
  {
    errx(1, "usage: %s solver or image-processing ...", argv[0]);
  }
  return 0;
}