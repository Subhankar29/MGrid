#ifndef DATASET_GENERATOR_H
#define DATASET_GENERATOR_H

#include <vector>

class DataSetGenerate {
public:
    DataSetGenerate(int n);

    std::vector<std::vector<double>> generate();

private:
    int n;
};

#endif // DATASET_GENERATOR_H
