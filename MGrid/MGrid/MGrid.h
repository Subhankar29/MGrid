#ifndef MGRID_MGRID_H
#define MGRID_MGRID_H

#include <vector>
#include "../Cluster/Cluster.h"
#include "../Pivot/Pivot.h"

struct Ring {
    double minDist;
    double maxDist;
};

class MGrid {
  private:
    vector<vector<double>> metricObjects;
    vector<Cluster> clusters;
    vector<Cluster> createCluster(vector<vector<double>> &metricObject,
                                  int numberOfCluster);
    map<int, vector<Ring>> mapOfPivotsToIndexRings;
    vector<vector<double>> pivots;
    map<int, map<int, vector<int>>> mapOfPivotIndexToMapOfRingsToDataIndexes;
    vector<Cluster> kMeansClustering(const vector<vector<double>> &data, int k,
                                     int maxIterations);
    map<int, map<int, pair<double, double>>>
    getMapOfPivotToListOfMinMaxDistancesToRings(
        map<int, map<int, vector<int>>>
            mapOfPivotIndexToMapOfRingsToDataIndexes,
        vector<vector<double>> data, vector<vector<double>> pivots);
    void getMetaDataCluster(vector<vector<double>> data,
                            vector<Cluster> *clusters,
                            map<int, map<int, pair<double, double>>>
                                *mapOfPivotToListOfMinMaxDistancesToRings);
    int numberOfPivots;
    int numberOfRings;

  public:
    explicit MGrid(const vector<vector<double>> &metricObject,
                   int numberOfPivots, int numberOfRings);
    virtual vector<Cluster> getCluster();
    map<int, vector<Ring>> getRingsForPivot();
    virtual vector<vector<double>> getPivots();
    map<int, map<int, vector<int>>>
    creatRings(vector<vector<double>> metricObject, vector<vector<double>>,
               int numberOfPivots);
    static int visitCluster(vector<vector<double>> data,
                            vector<double> queryObject, Cluster cluster,
                            double *currentNearestNeighbourDistance);
    static void nnSearchAlgorithm(vector<vector<double>> pivots,
                                  map<int, map<int, pair<double, double>>>
                                      *mapOfPivotToListOfMinMaxDistancesToRings,
                                  vector<double> queryObject,
                                  vector<vector<double>> data,
                                  vector<Cluster> clusters);

    static void pruneClusters(
        vector<double> queryObject,
        double radius,
        vector<Cluster>* activeClusters,
        map<int, map<int, pair<double, double>>> mapOfPivotToListOfMinMaxDistancesToRings,
        vector<vector<double>> pivots);
};

#endif //MGRID_MGRID_H
