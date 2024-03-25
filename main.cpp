#include <iostream>
#include "MGrid/MGrid/MGrid.h"
#include "Query/Query.h"
#include "Query/MGridQuery.h"
#include "GenerateData/DataSetGenerator.h"
#include <chrono>

void runTest();

using namespace std::chrono;

int main() {

    for (int i = 0; i < 10; i++) {
        runTest();
    }

    return 0;
}

void runTest() {
    // Generate the data set:
    DataSetGenerator dataSetGenerator(10000, 64);

    // Step 1: Get Data set
    vector<vector<double>> metricObjects = dataSetGenerator.generateDataNonUniformDistribution();

    // Step 2: Determine the number of Pivots and number of rings
    // Question: How we will determine it? What parameters should we consider?
    // to-do
    long numberOfPivots = 4;
    long numberOfRings = 5;
    long queryIndex = 50;
    long numberOfClusters = 20;

    auto start = std::chrono::high_resolution_clock::now();

    // Step 3: DataSetGenerator MGrid
    MGrid mGrid(metricObjects, numberOfPivots, numberOfRings, queryIndex, numberOfClusters);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    cout << "Time takes to run the whole query " << duration.count() << " ms" << endl;

    vector<vector<double>> pivots = mGrid.getPivots();

    vector<Cluster> clusters = mGrid.getCluster();

    // Step 4: Initialize the MGrid Query Object
    Query* mGridQuery = new MGridQuery(mGrid);

    // Step 5: Insert Objects to the MGrid:
    MetricObject objectToBeInserted;
    mGridQuery->insert(objectToBeInserted);
}
