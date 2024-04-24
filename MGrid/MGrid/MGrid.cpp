#include "MGrid.h"
#include "../Pivot/PivotIncrementalSelection.h"
#include <cmath>
#include <map>
#include <utility>
#include <chrono>
#include <algorithm>
#include <random>
#include <cstdlib>

using namespace std;

MGrid::MGrid(const vector<vector<double>> &metricObjects, const int numberOfPivots, const int numberOfRings,
             const int queryIndex, const int numberOfClusters)
        : metricObjects(metricObjects), numberOfPivots(numberOfPivots), numberOfRings(numberOfRings) {

    // Step 1: Select the Pivot using Incremental Selection algorithm
    int sample_size = 10;
    auto startPivots = std::chrono::high_resolution_clock::now();
    pivots = PivotIncrementalSelection::selectPivots(metricObjects, numberOfPivots, numberOfRings, sample_size);
    auto endPivots = std::chrono::high_resolution_clock::now();

    auto durationToSelectPivots = std::chrono::duration_cast<std::chrono::milliseconds>(endPivots - startPivots);

    cout << "Time taken to select the pivots: " << durationToSelectPivots.count() << " ms" << endl;

    //double queryDistance = PivotIncrementalSelection::vectorDistance(pivots[0], metricObjects[5]);

    // Step 2: Then select rings such that there are equal number of object in each rings for each pivot.
    //  mapOfPivotsToIndexRings = createRings(metricObjects, pivots, numberOfRings);

    auto startRings = std::chrono::high_resolution_clock::now();

    mapOfPivotIndexToMapOfRingsToDataIndexes = creatRings(metricObjects, pivots, numberOfRings);

    auto endRings = std::chrono::high_resolution_clock::now();

    auto durationToSelectRings = std::chrono::duration_cast<std::chrono::milliseconds>(endRings - startRings);

    cout << "Time taken to select the rings: " << durationToSelectRings.count() << " ms" << endl;

    map<int, map<int, pair<double, double>>> mapOfPivotToListOfMinMaxDistancesToRings =
            getMapOfPivotToListOfMinMaxDistancesToRings(
                    mapOfPivotIndexToMapOfRingsToDataIndexes,
                    metricObjects,
                    pivots
            );

    auto startCluster = std::chrono::high_resolution_clock::now();

    // Step 3: Cluster the object using K-means clustering Algorithm.
    // Create a map to store the cluster centroids
    clusters = Cluster::clusterData(metricObjects, numberOfClusters, 100);

    getMetaDataCluster(metricObjects, &clusters, &mapOfPivotToListOfMinMaxDistancesToRings);

    auto endCluster = std::chrono::high_resolution_clock::now();

    auto durationToClusterData = std::chrono::duration_cast<std::chrono::milliseconds>(endCluster - startCluster);

    cout << "Time taken to cluster the data: " << durationToClusterData.count() << " ms" << endl;

    auto durationToBuildMGrid = std::chrono::duration_cast<std::chrono::milliseconds>(endCluster - startPivots);

    cout << "Time taken to build MGrid: " << durationToBuildMGrid.count() << " ms" << endl;

    auto nnSearchStart = std::chrono::high_resolution_clock::now();

    // Seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define the range of noise
    std::uniform_real_distribution<double> dis(0.4, 0.4); // Range [-0.1, 0.1]

    auto queryObject = metricObjects[queryIndex];

    // Add noise to each data point
    for (double& point : queryObject) {
        double noise = dis(gen);
        point += noise;
    }

    // Objects are clustered based on their distances to pivots so objects occurring in the same rings
    // will be places in the same clusters.
    int nnResult = nnSearchAlgorithm(
                pivots,
                &mapOfPivotToListOfMinMaxDistancesToRings,
                queryObject,
                metricObjects,
                clusters,
                queryIndex
            );

    cout << "result " << nnResult << endl;

    auto nnSearchEnd = std::chrono::high_resolution_clock::now();

    auto durationForNNSearch = std::chrono::duration_cast<std::chrono::milliseconds>(nnSearchEnd - nnSearchStart);

    cout << "Time taken for NN search: " << durationForNNSearch.count() << " ms" << endl;
}

void MGrid::getMetaDataCluster(
        vector<vector<double>> data,
        vector<Cluster> *clusters,
        map<int, map<int, pair<double, double>>> *mapOfPivotToListOfMinMaxDistancesToRings) {

    for (auto &pivotToRingDistances: *mapOfPivotToListOfMinMaxDistancesToRings) {
        int pivotIndex = pivotToRingDistances.first;
        map<int, pair<double, double>> ringDistances = pivotToRingDistances.second;

        vector<double> pivot = data[pivotIndex];

        for (Cluster &cluster: *clusters) {
            double minDistance = numeric_limits<double>::max();
            double maxDistance = numeric_limits<double>::min();

            pair<int, int> ringMetaDataOfCluster;

            // Find the min and max positions from the pivot position
            for (int i = 0; i < cluster.listOfIndexes.size(); i++) {
                vector<double> dataPoint = data[cluster.listOfIndexes[i]];
                double distance = PivotIncrementalSelection::vectorDistance(dataPoint, pivot);

                if (i == 0) {
                    minDistance = distance;
                    maxDistance = distance;
                } else if (distance < minDistance) {
                    minDistance = distance;
                } else if (distance > maxDistance) {
                    maxDistance = distance;
                }
            }

            int ringStartId = -1;
            int ringEndId = -1;

            // Now, with the min and max distance we need to check in this pivot which all rings does this cluster lies.
            for (auto &ring: ringDistances) {
                int ringId = ring.first;
                double ringStartingPoint = ring.second.first;
                double ringEndingPoint = ring.second.second;

                if (minDistance >= ringStartingPoint && minDistance < ringEndingPoint) {
                    ringStartId = ringId;
                }

                if (maxDistance >= ringStartingPoint && maxDistance <= ringEndingPoint) {
                    ringEndId = ringId;
                }
            }

            ringMetaDataOfCluster.first = ringStartId;
            if (ringEndId == -1) {
                ringEndId = ringStartId;
            }
            ringMetaDataOfCluster.second = ringEndId;
            cluster.pivotToRingResidingOfCluster[pivotIndex] = ringMetaDataOfCluster;
        }
    }
}

void MGrid::pruneClusters(
        vector<double> queryObject,
        double radius,
        vector<Cluster> *activeClusters,
        map<int, map<int, pair<double, double>>> mapOfPivotToListOfMinMaxDistancesToRings,
        vector<vector<double>> pivots) {

    for (auto &outerIt: mapOfPivotToListOfMinMaxDistancesToRings) {
        int pivotId = outerIt.first;

        double distancePivotToQueryObject = PivotIncrementalSelection::vectorDistance(pivots[pivotId], queryObject);

        double querySpaceStartPoint = distancePivotToQueryObject - radius;
        double querySpaceEndPoint = distancePivotToQueryObject + radius;

        int querySpaceRingStartIndex = -1;
        int querySpaceRingEndIndex = -1;

        // Now identify the start and end ring index for the query Space
        map<int, pair<double, double>> ringMap = outerIt.second;

        for (auto &ringPair: ringMap) {
            int ringId = ringPair.first;
            double ringStartDistanceFromPivot = ringPair.second.first;
            double ringEndDistanceFromPivot = ringPair.second.second;

            if (querySpaceRingStartIndex == -1 && ringStartDistanceFromPivot <= querySpaceStartPoint &&
                querySpaceStartPoint <= ringEndDistanceFromPivot) {
                querySpaceRingStartIndex = ringId;
            }

            if (ringStartDistanceFromPivot <= querySpaceEndPoint) {
                querySpaceRingEndIndex = ringId;
            }

//      if (querySpaceRingStartIndex != -1 && querySpaceRingEndIndex != -1) {
//        break;
//      }
        }

        for (int i = 0; i < activeClusters->size(); i++) {
            int clusterStartRingId = (*activeClusters)[i].pivotToRingResidingOfCluster[pivotId].first;
            int clusterEndRingId = (*activeClusters)[i].pivotToRingResidingOfCluster[pivotId].second;

            if ((querySpaceRingStartIndex <= clusterStartRingId && clusterStartRingId <= querySpaceRingEndIndex)
                || (querySpaceRingStartIndex <= clusterEndRingId && clusterEndRingId <= querySpaceRingEndIndex)) {
                continue;
            } else {
                (*activeClusters)[i].isPruned = true;
            }
        }
    }

    return;
}

bool compareDist(const pair<Cluster, double> &p1, const pair<Cluster, double> &p2) {
    return p1.second < p2.second;
}

int MGrid::nnSearchAlgorithm(
        vector<vector<double>> pivots,
        map<int, map<int, pair<double, double>>> *mapOfPivotToListOfMinMaxDistancesToRings,
        vector<double> queryObject,
        vector<vector<double>> data,
        vector<Cluster> clusters,
        int queryIndex) {

    typedef pair<Cluster, double> clusterPair;
    vector<clusterPair> clusterDistanceArray;

    vector<Cluster> activeClusters;

    double nearestNeighbourDistance = numeric_limits<double>::max();
    double currentNearestNeighbourDistance = numeric_limits<double>::max();

    for (auto &cluster: clusters) {
        double distance = PivotIncrementalSelection::vectorDistance(cluster.mean, queryObject);
        clusterPair cp(cluster, distance);
        clusterDistanceArray.push_back(cp);
    }

    sort(clusterDistanceArray.begin(), clusterDistanceArray.end(), compareDist);

    activeClusters.reserve(clusterDistanceArray.size());
    // TODO: There are few of the clusters for which the cluster mean is not yet defined properly
    // hence we need to finish that.
    for (auto &cPair: clusterDistanceArray) {
        if (cPair.second > 0) {
            activeClusters.push_back(cPair.first);
        }
    }

    int i = 0;

    int nearestNeighbourIndex = visitCluster(data, queryObject, activeClusters[i], &currentNearestNeighbourDistance);
    nearestNeighbourDistance = currentNearestNeighbourDistance;

    pruneClusters(queryObject, nearestNeighbourDistance, &activeClusters, *mapOfPivotToListOfMinMaxDistancesToRings,
                  pivots);

    i++;

    auto averageTimeStart = std::chrono::high_resolution_clock::now();
    auto isFirstFound = false;


    // Prune the remaining cluster in a while loop.

    while (i < activeClusters.size()) {

        if (!activeClusters[i].isPruned) {
            nearestNeighbourIndex = visitCluster(data, queryObject, activeClusters[i],
                                                 &currentNearestNeighbourDistance);

            if (currentNearestNeighbourDistance < nearestNeighbourDistance) {
                nearestNeighbourDistance = currentNearestNeighbourDistance;
                pruneClusters(queryObject, nearestNeighbourDistance, &activeClusters,
                              *mapOfPivotToListOfMinMaxDistancesToRings, pivots);
            }
        }

	cout << "nearest neighbor index " << nearestNeighbourIndex << endl;

	double distance = PivotIncrementalSelection::vectorDistance(data[nearestNeighbourIndex], data[queryIndex]);
	cout << "Distance from nearestNeigh : " << distance << endl; 

    if (distance == 0.0 && !isFirstFound) {
		auto endTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - averageTimeStart);
		cout << "First correct value " << duration.count() << "ms" << endl;
		isFirstFound = true;

	}

        i++;
    }

    return nearestNeighbourIndex;

}

map<int, vector<Ring>> MGrid::getRingsForPivot() {
    return mapOfPivotsToIndexRings;
}

vector<Cluster> MGrid::getCluster() {
    return clusters;
}

vector<vector<double>> MGrid::getPivots() {
    return pivots;
}

map<int, map<int, vector<int>>> MGrid::creatRings(
        vector<vector<double>> metrics,
        vector<vector<double>> pivots,
        int numberOfRings) {

    // This creates a 3 dimension data object:
    // For each pivot, there is a vector of rings
    // The inner created an index of the ring and vector<vector<double>> represents the rings
    map<int, map<int, vector<int>>> mapOfPivotsToMapOfRingsIndexesToTheDataPoints;

    int ringSegmentCount = 1;

    // For each pivot calculate the distance to each object
    for (int i = 0; i < pivots.size(); i++) {
        vector<pair<double, vector<double>>> distances;

        // List Of Pair of index of the pivot to the list of points
        map<int, vector<double>> mapOfPivotIndexToPoints;

        // List of distances from this pivot to the metrics
        vector<pair<int, double>> distancesOfIndexDataToPivots;

        for (size_t m = 0; m < metrics.size(); m++) {
            double distanceOfDataFromPivot = PivotIncrementalSelection::vectorDistance(metrics[m], pivots[i]);
            pair<int, double> distanceOfObjectToCurrentPivot(m, distanceOfDataFromPivot);
            distancesOfIndexDataToPivots.push_back(distanceOfObjectToCurrentPivot);
        }

        std::sort(distancesOfIndexDataToPivots.begin(), distancesOfIndexDataToPivots.end(),
                  [](const auto &a, const auto &b) {
                      return a.second < b.second;
                  }
        );

        // Identify the number of points in each rings:
        //  if there are 10 rings and 100 points then each rings will have 10 points.
        int totalNumberOfPointsInOneSegment = metrics.size() / numberOfRings;
        bool isOdd = metrics.size() % 2 != 0;
        map<int, vector<int>> mapOfRingIndexToDataPoints;
        vector<int> listOfDataPointsInARingSegment;

        for (auto &distancesOfIndexDataToPivot: distancesOfIndexDataToPivots) {
            int indexOfDataPoint = distancesOfIndexDataToPivot.first;
            listOfDataPointsInARingSegment.push_back(indexOfDataPoint);

            if (isOdd) {
                if (ringSegmentCount != numberOfRings &&
                    listOfDataPointsInARingSegment.size() == totalNumberOfPointsInOneSegment) {
                    mapOfRingIndexToDataPoints[ringSegmentCount] = listOfDataPointsInARingSegment;
                    listOfDataPointsInARingSegment.clear();
                    ringSegmentCount++;
                } else if (ringSegmentCount == numberOfRings &&
                           listOfDataPointsInARingSegment.size() == totalNumberOfPointsInOneSegment + 1) {
                    mapOfRingIndexToDataPoints[ringSegmentCount] = listOfDataPointsInARingSegment;
                    listOfDataPointsInARingSegment.clear();
                }
            } else {
                if (listOfDataPointsInARingSegment.size() == totalNumberOfPointsInOneSegment) {
                    mapOfRingIndexToDataPoints[ringSegmentCount] = listOfDataPointsInARingSegment;
                    listOfDataPointsInARingSegment.clear();
                    ringSegmentCount++;
                }
            }
        }

        mapOfPivotsToMapOfRingsIndexesToTheDataPoints[i] = mapOfRingIndexToDataPoints;
    }

    return mapOfPivotsToMapOfRingsIndexesToTheDataPoints;
}

map<int, map<int, pair<double, double>>> MGrid::getMapOfPivotToListOfMinMaxDistancesToRings(
        map<int, map<int, vector<int>>> mapOfPivotIndexToMapOfRingsToDataIndexes,
        vector<vector<double>> data,
        vector<vector<double>> pivots) {
    map<int, map<int, pair<double, double>>> mapOfPivotToListOfMinMaxDistancesToRings;

    bool isFirst = true;

    for (const auto &pivotToRingsMap: mapOfPivotIndexToMapOfRingsToDataIndexes) {
        int pivotIndex = pivotToRingsMap.first;
        map<int, vector<int>> ringsToListOfIndexes = pivotToRingsMap.second;
        map<int, pair<double, double>> ringToMinMaxDistance;

        double minDistance;
        double maxDistance;

        for (const auto &ringsToIndex: ringsToListOfIndexes) {
            int ringIndex = ringsToIndex.first;
            vector<int> listOfObjectInTheRing = ringsToIndex.second;

            if (isFirst) {
                minDistance = 0;
                isFirst = false;
            }

            int indexOfLastObjectInRing = listOfObjectInTheRing.size() - 1;

            maxDistance = PivotIncrementalSelection::vectorDistance(
                    pivots[pivotIndex],
                    data[listOfObjectInTheRing[indexOfLastObjectInRing]]
            );

            pair<double, double> minAndMaxDistanceOfRingFromPivot = {minDistance, maxDistance};
            ringToMinMaxDistance[ringIndex] = minAndMaxDistanceOfRingFromPivot;

            minDistance = maxDistance;
        }
        mapOfPivotToListOfMinMaxDistancesToRings[pivotIndex] = ringToMinMaxDistance;
    }

    return mapOfPivotToListOfMinMaxDistancesToRings;
}

int MGrid::visitCluster(
        vector<vector<double>> data,
        vector<double> queryObject,
        Cluster cluster,
        double *currentNearestNeighbourDistance) {
    int nearestNeighborObject;
    for (auto index: cluster.listOfIndexes) {
        double distance = PivotIncrementalSelection::vectorDistance(queryObject, data[index]);
        if (distance < *currentNearestNeighbourDistance) {
            nearestNeighborObject = index;
            *currentNearestNeighbourDistance = distance;
        }
    }

    return nearestNeighborObject;
}


/*
map<int, vector<Ring>> MGrid::createRings(
        vector<vector<double>> metrics,
        vector<vector<double>> pivots,
        int numberOfRings) {

map<int, vector<Ring>> ringMap;

for (int i = 0; i < pivots.size(); i++) {

  // Get distances to pivot
  std::vector<double> distances;
  for (auto& point : metrics) {
    double distance = PivotIncrementalSelection::vectorDistance(point, pivots[i]);
    distances.push_back(distance);
  }

  // Determine min and max dist
  double minDist = *std::min_element(distances.begin(), distances.end());
  double maxDist = *std::max_element(distances.begin(), distances.end());

  // Compute interval size
  double interval = (maxDist - minDist) / numberOfRings;

  // Generate ring distances
  vector<Ring> ringDist;
  ringDist.reserve(numberOfRings);
  Ring prev{minDist , minDist};

  for (int r = 0; r < numberOfRings; r++) {
    Ring ring{};
    ring.minDist = prev.maxDist;
    ring.maxDist = ring.minDist + interval;

    ringDist.push_back(ring);
    prev = ring;
  }

  ringMap[i] = ringDist;
}

return ringMap;
}
*/
