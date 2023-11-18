#include "GenerateData/DataSetGenerator.h"
#include "MGrid/MGrid/MGrid.h"
#include "MGrid/Pivot/PivotIncrementalSelection.h"
#include "Query/MGridQuery.h"
#include "Query/Query.h"
#include <chrono>
#include <iostream>
#include <numeric>

using namespace std::chrono;

using namespace std;

int main() {

    auto start = std::chrono::high_resolution_clock::now();

    // Step 3: DataSetGenerator MGrid

    vector<long> pivotTime;
    vector<long> ringTime;
    vector<long> clusterTime;
    vector<long> mGridTime;
    vector<long> correctAns;
    vector<long> nnSearchTime;

    vector<int> numberOfObjects = {100, 10000, 50000, 100000, 250000, 500000, 750000, 1000000};

    for (int i = 0; i < numberOfObjects.size(); i++) {
      // Step 2: Determine the number of Pivots and number of rings
      // Question: How we will determine it? What parameters should we consider?
      // to-do
      long numberOfPivots = 10;
      long numberOfRings = 10;
      long queryIndex =  numberOfObjects[i] - 1;
      long numberOfClusters = 100;

      cout << " number of objects " <<  numberOfObjects[i] << endl;
      cout << " Number of pivots : " << numberOfPivots << " number of Rings  " << numberOfRings << " number of Clusters " <<  numberOfClusters << endl;

      // Generate the data set:
      DataSetGenerator dataSetGenerator(numberOfObjects[i], 64);

      // Step 1: Get Data set
      vector<vector<double>> metricObjects = dataSetGenerator.generateDataNonUniformDistribution();
//      vector<vector<double>> metricObjects = dataSetGenerator.generateDataUsingUniformDistribution();
      MGrid mGrid(metricObjects, numberOfPivots, numberOfRings, queryIndex, numberOfClusters);

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

      cout << "Result from linear scan" << endl;

      auto linearScanStartTime = std::chrono::high_resolution_clock::now();

      for (int n = 0; n < metricObjects.size(); n++) {
        double linearScanDistance = PivotIncrementalSelection::vectorDistance(metricObjects[i], metricObjects[queryIndex]);

        if (linearScanDistance == 0) {
          auto linearScanQueryFoundEndTime = std::chrono::high_resolution_clock::now();
          auto durationOfLinearScan = std::chrono::duration_cast<std::chrono::milliseconds>(linearScanQueryFoundEndTime - linearScanStartTime);
          cout << "Time taken to scan the object " << durationOfLinearScan.count() << " ms " << endl;
          break;
        }
      }

      auto linearScanCompleted = std::chrono::high_resolution_clock::now();
      auto durationOfTotalLinearScan = std::chrono::duration_cast<std::chrono::milliseconds>(linearScanCompleted - linearScanStartTime);

      cout << "Time taken to scan the file completely " << durationOfTotalLinearScan.count() << " ms " << endl;
      cout << "------------------------------------------------------------------------" << endl;
    }

//    long avgPivotTime = accumulate(pivotTime.begin(), pivotTime.end(), 0) / pivotTime.size();
//
//    cout << "Average Time taken to select the pivots: " << avgPivotTime << " ms" << endl;
//
//    long avgRingTime = accumulate(ringTime.begin(), ringTime.end(), 0)/ ringTime.size();
//
//    cout << "Average Time taken to form the rings:  " << avgRingTime << " ms" << endl;
//
//    long avgClusterTime  = accumulate(clusterTime.begin(), clusterTime.end(), 0) / clusterTime.size();
//
//    cout << "Average Time taken to cluster the data: " << avgClusterTime << " ms" << endl;
//
//    long avgMGridTime = accumulate(mGridTime.begin(), mGridTime.end(), 0)/ mGridTime.size();
//
//    cout << "Average Time takes to form MGrid " << avgMGridTime << " ms" << endl;
//
//    double avgCorrectAns = (double) (accumulate(correctAns.begin(), correctAns.end(), 0)) / (double) correctAns.size();
//
//    cout << "Total correct " << accumulate(correctAns.begin(), correctAns.end(), 0) << " percentage "
//         << (double) (accumulate(correctAns.begin(), correctAns.end(), 0) / (double) correctAns.size()) << endl;
//
//    cout << "Percent of correct ans " << avgCorrectAns * 100 << " %" << endl;
//
//    long avgNNSearchTime = accumulate(nnSearchTime.begin(), nnSearchTime.end(), 0) / nnSearchTime.size();
//
//    cout << "Average time for nnSearch " << avgNNSearchTime << " ms" << endl;
//
//    auto end = std::chrono::high_resolution_clock::now();
//
//    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
//
//    cout << "Time takes to run the whole query " << duration.count() << " ms" << endl;

    return 0;
}
