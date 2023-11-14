#ifndef DATASET_GENERATOR_H
#define DATASET_GENERATOR_H

#include <vector>

class DataSetGenerator {
public:
    DataSetGenerator(int n);

    std::vector<std::vector<double>> generateDataUsingUniformDistribution() const;

private:
    int n;
};

#endif // DATASET_GENERATOR_H
