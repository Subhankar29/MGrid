#include <iostream>
#include "MGrid/MGrid/MGrid.h"
#include "Query/Query.h"
#include "Query/MGridQuery.h"
#include "GenerateData/DataSetGenerator.h"

int main() {

    // Generate the data set:
    DataSetGenerator dataSetGenerator(7);

    // Step 1: Get Data set
    vector<vector<double>> metricObjects = dataSetGenerator.generateDataUsingUniformDistribution();
    //

//    vector<MetricObject> metricObjects;

    // Step 2: Determine the number of Pivots and number of rings
    // Question: How we will determine it? What parameters should we consider?
    // to-do
    long numberOfPivots = 1;
    long numberOfRings = 3;

    // Step 3: DataSetGenerator MGrid
    MGrid mGrid(metricObjects, numberOfPivots, numberOfRings);

    vector<vector<double>> pivots = mGrid.getPivots();


    vector<Cluster> clusters = mGrid.getCluster();

    // Step 4: Initialize the MGrid Query Object
    Query* mGridQuery = new MGridQuery(mGrid);

    // Step 5: Insert Objects to the MGrid:
    MetricObject objectToBeInserted;
    mGridQuery->insert(objectToBeInserted);

    // Step 6: Construct the query parameter
    // Question: In the query parameter, what is the query object? Is it a metricObject.
    // How we will decide the query parameters?
    // How to determine the radius?
//    MetricObject queryObject;
//    float radius = 0;
//
//    QueryParameter queryParameter(queryObject, pivots, clusters, radius);
//
//    // Step 7: search object
//    vector<MetricObject> searchedObject = mGridQuery->search(queryParameter);
//
//    // Step 8: delete inserted object
//    MetricObject objectToBeDeleted = searchedObject[0];
//    mGridQuery->deleteObject(objectToBeDeleted);

    return 0;
}
