#include <random>
#include "DataSetGenerator.h"

using namespace std;

DataSetGenerator::DataSetGenerator(int n) : n(n) {}

vector<vector<double>> DataSetGenerator::generateDataUsingUniformDistribution() const {
    vector<vector<double>> data(n);

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < n; i++) {
        data[i].resize(n);
        for (int j = 0; j < n; j++) {
            data[i][j] = dis(gen);
        }
    }

    return data;
}
