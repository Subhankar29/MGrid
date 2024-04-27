#include <iostream>
#include "MGrid/MGrid/MGrid.h"
#include "Query/Query.h"
#include "Query/MGridQuery.h"
#include "GenerateData/DataSetGenerator.h"
#include <chrono>

using namespace std::chrono;
void run(int dim, int clusterSize);

int main() {

    auto dimensions = { 1024 };
    auto number_of_rings = { 2, 4, 7, 10, 13 };

    run(4096, 4);

    return 0;
}

void run(int dimension,
         int pivot) {
    // Generate the data set:
    cout << "------------------- START ----------------" << endl;
    cout << "pivot count : " <<  pivot << endl;
    DataSetGenerator dataSetGenerator(100000, dimension);

    // Step 1: Get Data set
    vector<vector<double>> metricObjects = dataSetGenerator.generateDataNonUniformDistribution();

    // Step 2: Determine the number of Pivots and number of rings
    // Question: How we will determine it? What parameters should we consider?
    // to-do
    long numberOfPivots = 5;
    long numberOfRings = 5;
    long queryIndex = 1000;
    long numberOfClusters = 10;

    // auto end_linear = std::chrono::high_resolution_clock::now();

    // auto duration_linear = std::chrono::duration_cast<std::chrono::milliseconds>(end_linear - start_linear);

    // cout<<"Time taken for linear scan" << duration_linear.count() << " ms " << endl;

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

    cout << "------------------END FILE ---------------------------" << endl;
}
