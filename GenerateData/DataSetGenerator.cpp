#include <random>
#include "DataSetGenerator.h"

using namespace std;

DataSetGenerator::DataSetGenerator(int numberOfRows, int numberOfCols) : numberOfRows(numberOfRows), numberOfCols(numberOfCols) {}

vector<vector<double>> DataSetGenerator::generateDataUsingUniformDistribution() const {
    vector<vector<double>> data(numberOfRows);

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < numberOfRows; i++) {
        data[i].resize(numberOfCols);
        for (int j = 0; j < numberOfCols; j++) {
            data[i][j] = dis(gen);
        }
    }

    return data;
}

vector<vector<double>> DataSetGenerator::generateDataNonUniformDistribution() const  {
    vector<vector<double>> data(numberOfRows, vector<double>(numberOfCols));

    // Parameters for Gaussian mixture model
    std::vector<double> means = {-2, 2};
    std::vector<double> stdevs = {1, 0.5};
    std::vector<double> weights = {0.3, 0.7};

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0,1.0);

    for(int i=0; i< numberOfRows; ++i) {

        // Sample component from mixture weights
        std::discrete_distribution<> component(weights.begin(), weights.end());
        int k = component(generator);

        // Sample point from k-th Gaussian
        for(int j=0; j < numberOfCols; ++j) {
            double mean = means[k];
            double stddev = stdevs[k];
            data[i][j] = mean + distribution(generator) * stddev;
        }

    }

    return data;
}
