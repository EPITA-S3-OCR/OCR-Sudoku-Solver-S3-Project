
#include "stdlib.h"
#include "stdio.h"
#include "math.h"


// We want a neural network that learns XOR.

// Layers size 
#define inputNum 2
#define hidenNodesNum 2
#define outputNum 1

#define trainingSetNum 4


double sigmoidFunction(double x){
    return 1/(1+exp(-x));
}

double derivativeSigmoidFunction(double x){
    return x * (1-x);
}

double initWeights(){
    return (double)rand()/(double)RAND_MAX;
}

void shuffleArray(int *array, size_t n){
    if (n > 1){
        for (size_t i = 0; i < n-1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n-i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}


int main (int argc, char *argv[])
{

    const double lr = 0.1f;

    double hiddenLayer[hidenNodesNum];
    double outputLayer[outputNum];

    double hiddenLayerBiases[hidenNodesNum];
    double outputLayerBias[outputNum];

    // Because they are fully connected.
    double hiddenWeights[inputNum][hidenNodesNum];
    double outputWeights[hidenNodesNum][outputNum];

    
    double trainingInputs[trainingSetNum][inputNum] = {{0.0f,0.0f},
                                                       {0.0f,1.0f},
                                                       {1.0f,0.0f},
                                                       {1.0f,1.0f}};

    
    double trainingOutputs[trainingSetNum][outputNum] = {{0.0f},
                                                        {1.0f},
                                                        {1.0f},
                                                        {0.0f}};



    // Initialize the weights.

    for (size_t i = 0; i < hidenNodesNum; i++) {
        for (size_t j = 0; j < outputNum; j++)
            outputWeights[i][j] = initWeights(); 
    }

    for (size_t i = 0; i < inputNum; i++) {
        for (size_t j = 0; j < hidenNodesNum; j++)
            hiddenWeights[i][j] = initWeights();  
    }

    // Initialize biases.
    //
    
    for (size_t i = 0; i < outputNum; i++) {
       outputLayerBias[i] = initWeights(); 
    }


    int trainingSetOrder[] = {0,1,2,3};

    int numberOfEpochs = 10000;


    // Now we actually train it.

    for (size_t epoch = 0; epoch < numberOfEpochs; epoch++) {

        shuffleArray(trainingSetOrder, trainingSetNum);

        for (size_t x = 0; x < trainingSetNum; x++) {

           int i = trainingSetOrder[x];

           // Forward pass

           // Hidden layer activation

           for(int j = 0; j < hidenNodesNum; j++){

               double activation = hiddenLayerBiases[j];

               for(int k = 0; k < inputNum; k++){
                   activation += trainingInputs[i][k] * hiddenWeights[k][j];
               }

               hiddenLayer[j] = sigmoidFunction(activation);


           }
           // Output layer activation

           for(int j = 0; j < outputNum; j++){

               double activation = outputLayerBias[j];

               for(int k = 0; k < hidenNodesNum; k++){
                   activation += hiddenLayer[k] * outputWeights[k][j];
               }

               outputLayer[j] = sigmoidFunction(activation);

           }

           printf("Input: %g %g    Output: %g         Predicted output: %g\n",
                   trainingInputs[i][0],trainingInputs[i][1], outputLayer[0],
                   trainingOutputs[i][0]);

           // Backpropagation part.

           // Change in output weights.

           double deltaOutput[outputNum];

           for (int j = 0; j < outputNum; j++) {
               double error = trainingOutputs[i][j] - outputLayer[j];
               deltaOutput[j] = error * derivativeSigmoidFunction(outputLayer[j]);
           }

           // Change in hidden weights.
           double deltaHidden[hidenNodesNum];

           for (int j = 0; j < hidenNodesNum; j++) {
               double error = 0.0f;
               for(int k = 0; k < outputNum; k ++){
                   error += deltaOutput[k] * outputWeights[j][k];
               }
               deltaHidden[j] = error * derivativeSigmoidFunction(hiddenLayer[j]);
           }


           // Apply the changes in the weights.

           // Change the output weights.

           for (size_t j = 0; j < outputNum; j++) {
               outputLayerBias[j] += deltaOutput[j] * lr;
               for (size_t k = 0; k < hidenNodesNum; k++) {
                   outputWeights[k][j] += hiddenLayer[k] * deltaOutput[j] * lr;
               }
           }

           // Change the hidden layers weights

           for (size_t j = 0; j < hidenNodesNum; j++) {
               hiddenLayerBiases[j] += deltaHidden[j] * lr;
               for (size_t k = 0; k < inputNum; k++) {
                   hiddenWeights[k][j] += trainingInputs[i][k] * deltaHidden[j] * lr;
               }
           }


        }



    }


    return 0;
}
