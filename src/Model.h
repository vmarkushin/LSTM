//
// Created by negig on 23.02.2017.
//

#ifndef LSTM_MODEL_H
#define LSTM_MODEL_H


#include "utils/matrix.h"
class Model;

enum LayerType {
    INPUT = 0,
    HIDDEN,
    RECURRENT,
    LSTM,
    OUTPUT
};

class Layer {
public:
    LayerType type;
    Layer(const unsigned int in, const unsigned int out);
    virtual void feedForward(Model& model, Layer& prevLayer);
    virtual void backPropagate(Model& model, Layer& nextLayer, const unsigned int& step);
    virtual void backPropagate(Model& model, Layer& nextLayer, matrix& XSet, const unsigned int& step);
    matrix W, deltaW, updateW;
    matrix b, deltaB, updateB;
    matrix outputValues;
    matrix v; // for momentum
    std::vector<matrix> gradients;
};

class RNNLayer : public Layer {
public:
    RNNLayer(const unsigned int in, const unsigned int out, int capacity);
    void feedForward(Model& model, Layer& prevLayer);
    void backPropagate(Model& model, Layer& nextLayer, const unsigned int& step);
    matrix memoryW, gradientMemoryW, updateMemoryW;
    matrix memoryV; // for momentum
    std::vector<matrix> prevOutputValues;
    int capacity;
};

class LSTMLayer : public RNNLayer {
public:
    LSTMLayer(const unsigned int prevIn, const unsigned int in, const unsigned int out, int capacity);
    void feedForward(Model& model, Layer& prevLayer);
    void backPropagate(Model& model, Layer& nextLayer, const unsigned int& step);
    // Cell State
    matrix cellState;
    matrix cellStateW, gradientCellStateW, updateCellStateW;
    matrix cellStateB, deltaCellStateB, updateCellStateB;
    // Candidate Cell State
    matrix candidateCellState;
    matrix candidateCellStateW, gradientCandidateCellStateW, updateCandidateCellStateW;
    matrix candidateCellStateB, deltaCandidateCellStateB, updateCandidateCellStateB;
    std::vector<matrix> prevCellsStateValues;
    // Forget Gate layer
    matrix forgetW, gradientForgetW, updateForgetW;
    matrix forgetB, deltaForgetB, updateForgetB;
    // Input Gate layer
    matrix inputGateW, gradientInputGateW, updateInputGateW;
    matrix inputGateB, deltaInputGateB, updateInputGateB;
    // Sigmoid Gate layer
    matrix sigmoidGateW, gradientSigmoidGateW, updateSigmoidGateW;
    matrix sigmoidGateB, deltaSigmoidGateB, updateSigmoidGateB;

};

class Model {
    friend class Layer;
    friend class RNNLayer;
    friend class LSTMLayer;

public:
    Model(double learningRate, double momentum);
    void addLayer(Layer* layer);
    void train(std::vector<matrix>& XSet, std::vector<matrix>& ySet, double* predicted);
    matrix getOutputValues();
    ~Model();

    double mu = 0.3f;
    double eta = 1.5f;
private:
    std::vector<Layer*> layers;
    matrix* outputValues;
    int recurrentSteps = 0;

    double overallError = 0;
    double error, recentAverageError = 0;
public:
    double getRecentAverageError() const;

private:

    const double recentAverageSmoothingFactor = 100.0;
};


#endif //LSTM_MODEL_H
