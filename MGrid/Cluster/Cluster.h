#ifndef MGRID_CLUSTER_H
#define MGRID_CLUSTER_H

#include "../../MetricStructs/MetricObject.h"
#include <map>
#include <vector>
#include <string>

using namespace std;

class Cluster {
public:
    vector<int> listOfIndexes;
    vector<double> mean;
    vector<double> centroid;
    vector<vector<double>> points;
    map<int, pair<int, int>> pivotToRingResidingOfCluster;
    bool isPruned;
    int centroidIndex;

public:
    map<int, vector<int>> getMetaData();

    vector<double> getMean();

    static vector<Cluster> clusterData(vector<vector<double>> data, int k, int maxIterations);

    static vector<vector<double>> getCentroids(vector<vector<double>> data, int k, int maxIterations);

    static vector<Cluster> kMeansClustering(vector<vector<double>>* data, int k, int maxIterations);
};

#endif //MGRID_CLUSTER_H
