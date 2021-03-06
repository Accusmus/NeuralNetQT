#ifndef BACKPROPAGATION_H
#define BACKPROPAGATION_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <QDebug>

//-----------------------
//file manipulation
#include <QFile>
#include <QTextStream>
#include <QDataStream>
//-----------------------

#include "globalVariables.h"



class Backpropagation
{
public:
    Backpropagation();
    void initialise();
    void saveWeights(QString fileName);
    void loadWeights(QString fileName);
    void feedForward( );
    void backPropagate();
    double sigmoid( double val );
    double reluActivation(double val);
    double tanhActivation(double val);
    double softmax(double val, double total);
    double sigmoidDerivative( double val );
    double reluDerivative(double val);
    double tanhDerivative(double val);
    void assignRandomWeights( void );
    double trainNetwork(int NUMBER_OF_DESIRED_EPOCHS);
    double getError_SSE();
    double getError_MSE();
    double getCorrect();
    double getError_PG();
    int action( double *vector );
    double* testNetwork(LetterStructure testPattern);

private:

    /* Weight Structures */

    /* Input to Hidden Weights (with Biases) */
    double wih[INPUT_NEURONS+1][HIDDEN_NEURONS];

    /* Hidden to Output Weights (with Biases) */
    double who[HIDDEN_NEURONS+1][OUTPUT_NEURONS];

    /* Activations */
    double inputs[INPUT_NEURONS];
    double hidden[HIDDEN_NEURONS];
    double target[OUTPUT_NEURONS];
    double actual[OUTPUT_NEURONS];

    /* Unit Errors */
    double erro[OUTPUT_NEURONS];
    double errh[HIDDEN_NEURONS];

    void resetConfusionMatrix();
    void logConfusionMatrixToFile();

    //-----------------------------------------
    double sse, mse, percentCorrect, pgood;

    int confusionMatrix[OUTPUT_NEURONS][OUTPUT_NEURONS]; //x array is actual y direction is expected

    int i, sample, iterations;
    int sum;
};

#endif // BACKPROPAGATION_H
