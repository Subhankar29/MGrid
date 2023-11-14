#ifndef DATASET_GENERATOR_H
#define DATASET_GENERATOR_H

#include <vector>

class DataSetGenerator {
public:
    DataSetGenerator(int n, int m);

    std::vector<std::vector<double>> generateDataUsingUniformDistribution() const;

    std::vector<std::vector<double>> generateDataNonUniformDistribution() const;

private:
    int numberOfRows;
    int numberOfCols;
};

#endif // DATASET_GENERATOR_H
