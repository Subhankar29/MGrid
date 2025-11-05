#include "Cluster.h"
#include "random"
#include <cfloat>
#include <limits>
#include "../Pivot/PivotIncrementalSelection.h"

vector<Cluster> Cluster::clusterData(vector<vector<double>> data, int k, int maxIterations) {
    vector<vector<double>> centroids = getCentroids(data, k, maxIterations);
    vector<Cluster> clusters(centroids.size());

    for (int i = 0; i < data.size(); i++) {
      double minDistance = numeric_limits<double>::max();
      int assignedClusterIndex = -1;
      for (int c = 0; c < centroids.size(); c++) {
        clusters[c].mean = centroids[c];
        double distance = PivotIncrementalSelection::vectorDistance(data[i], centroids[c]);
        if (distance < minDistance){
          minDistance = distance;
          assignedClusterIndex = c;
        }
      }

      clusters[assignedClusterIndex].listOfIndexes.push_back(i);
    }

    return clusters;
}

vector<vector<double>> Cluster::getCentroids(vector<vector<double>> data, int k, int maxIterations) {

  vector<vector<double>> centroids;
  vector<double> centroid(data[0].size(), 0);

  // Initialize centroids randomly
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, data.size() - 1);
  for(int i = 0; i < k; ++i) {
    int centroidIndex = dis(gen);
    centroid = data[centroidIndex];
    centroids.push_back(centroid);
  }

  bool stable = false;
  vector<int> cluster(data.size(), -1);
  int iterations = 0;

  while(!stable && iterations < maxIterations) {
    stable = true;
    iterations++;

    // Assign points to nearest centroid
    for(int i = 0; i < data.size(); ++i) {
      double minDist = DBL_MAX;
      int minIndex = -1;

      for(int j = 0; j < centroids.size(); ++j) {
        double dist = 0.0;
        for(int d = 0; d < data[i].size(); ++d) {
          dist += (data[i][d] - centroids[j][d]) * (data[i][d] - centroids[j][d]);
        }
        if(dist < minDist) {
          minDist = dist;
          minIndex = j;
        }
      }

      if(cluster[i] != minIndex) {
        stable = false;
        cluster[i] = minIndex;
      }
    }

    // Update centroids to cluster means
    for(int j = 0; j < k; ++j) {
      centroid = vector<double>(data[0].size(), 0);
      int count = 0;
      for(int i = 0; i < data.size(); ++i) {
        if(cluster[i] == j) {
          for(int d = 0; d < data[i].size(); ++d) {
            centroid[d] += data[i][d];
          }
          count++;
        }
      }
      if (count > 0) {
        for(int d = 0; d < centroid.size(); ++d) {
          centroid[d] /= count;
        }
      }
      centroids[j] = centroid;
    }
  }

  return centroids;
}