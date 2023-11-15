#include <iostream>
#include "MGrid/MGrid/MGrid.h"
#include "Query/Query.h"
#include "Query/MGridQuery.h"
#include "GenerateData/DataSetGenerator.h"
#include <chrono>
#include <numeric>

using namespace std::chrono;

using namespace std;

int main() {

    // Generate the data set:
    DataSetGenerator dataSetGenerator(100000, 64);

    // Step 1: Get Data set
    vector<vector<double>> metricObjects = dataSetGenerator.generateDataNonUniformDistribution();

    // Step 2: Determine the number of Pivots and number of rings
    // Question: How we will determine it? What parameters should we consider?
    // to-do
    long numberOfPivots = 3;
    long numberOfRings = 5;
    long queryIndex = 100;
    long numberOfClusters = 20;

    auto start = std::chrono::high_resolution_clock::now();

    // Step 3: DataSetGenerator MGrid
    MGrid mGrid(metricObjects, numberOfPivots, numberOfRings, queryIndex, numberOfClusters);

    vector<long> pivotTime;
    vector<long> ringTime;
    vector<long> clusterTime;
    vector<long> mGridTime;
    vector<long> correctAns;
    vector<long> nnSearchTime;

    for (int i = 0; i < 10; i++) {
        vector<long> results = mGrid.buildAndSearch(queryIndex);

        cout << "------------------------------------------------------------------------" << endl;

        cout << "Time taken to select the pivots: " << results[0] << " ms" << endl;
        pivotTime.push_back(results[0]);
        cout << "Time taken to select the rings: " << results[1] << " ms" << endl;
        ringTime.push_back(results[1]);
        cout << "Time taken to cluster the data: " << results[2] << " ms" << endl;
        clusterTime.push_back(results[2]);
        cout << "Time taken to build MGrid: " << results[3] << " ms" << endl;
        mGridTime.push_back(results[3]);
        cout << "result " << results[4] << endl;
        if (results[4] == queryIndex) {
            correctAns.push_back(1);
        } else {
            correctAns.push_back(0);
        }
        cout << "Time taken for NN search: " << results[5] << " ms" << endl;
        nnSearchTime.push_back(results[5]);
        cout << "------------------------------------------------------------------------" << endl;

    }

    long avgPivotTime = accumulate(pivotTime.begin(), pivotTime.end(), 0) / pivotTime.size();

    cout << "Average Time taken to select the pivots: " << avgPivotTime << " ms" << endl;

    long avgRingTime = accumulate(ringTime.begin(), ringTime.end(), 0)/ ringTime.size();

    cout << "Average Time taken to form the rings:  " << avgRingTime << " ms" << endl;

    long avgClusterTime  = accumulate(clusterTime.begin(), clusterTime.end(), 0) / clusterTime.size();

    cout << "Average Time taken to cluster the data: " << avgClusterTime << " ms" << endl;

    long avgMGridTime = accumulate(mGridTime.begin(), mGridTime.end(), 0)/ mGridTime.size();

    cout << "Average Time takes to form MGrid " << avgMGridTime << " ms" << endl;

    double avgCorrectAns = (double) (accumulate(correctAns.begin(), correctAns.end(), 0)) / (double) correctAns.size();

    cout << "Total correct " << accumulate(correctAns.begin(), correctAns.end(), 0) << " percentage "
    << (double) (accumulate(correctAns.begin(), correctAns.end(), 0) / (double) correctAns.size()) << endl;

    cout << "Percent of correct ans " << avgCorrectAns * 100 << " %" << endl;

    long avgNNSearchTime = accumulate(nnSearchTime.begin(), nnSearchTime.end(), 0) / nnSearchTime.size();

    cout << "Average time for nnSearch " << avgNNSearchTime << " ms" << endl;

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    cout << "Time takes to run the whole query " << duration.count() << " ms" << endl;

    return 0;
}
