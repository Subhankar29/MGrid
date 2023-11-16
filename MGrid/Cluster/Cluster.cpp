#include "Cluster.h"
#include "random"
#include <cfloat>
#include "../Pivot/PivotIncrementalSelection.h"

struct Point {
  int index;
  int clusterId = -1;
  double minDist = DBL_MAX;
};

vector<Cluster> Cluster::kMeansClustering(vector<vector<double>>* data, int k, int maxIterations) {
  // Initialise the centroids
  vector<Cluster> clusters(k);
  vector<Point> points(data->size());

  int dataSize = (*data).size();

  srand(time(0));

  for (int i = 0; i  < k; ++i) {
    clusters[i].centroid = data->at(rand() % dataSize);
  }

  int count = 0;
  while (count < maxIterations){
    count++;

    // set datapoint to the nearest index:
    for (int c = 0; c < clusters.size(); c++) {
      for (int p = 0; p < dataSize; p++) {
        double dist = PivotIncrementalSelection::vectorDistance(clusters[c].centroid, (*data)[p]);

        if (dist < points[p].minDist) {
          points[p].minDist = dist;
          points[p].clusterId = c;
        }
      }
    }

    for (int c = 0; c < clusters.size(); c++) {
      clusters[c].listOfIndexes = {};
    }

    // Append the centroid
    for (int p = 0; p < dataSize; p++) {
      int clusterId = points[p].clusterId;

      for (int d = 0; d < (*data)[0].size(); d++) {
        clusters[clusterId].centroid[d] += (*data)[p][d];
      }

      clusters[clusterId].listOfIndexes.push_back(p);
      points[p].minDist = DBL_MAX;
    }

    // Compute the new centroids
    for (int c = 0; c < clusters.size(); c++) {
      double total;
      for (int m = 0; m < clusters[c].centroid.size(); m++) {
        clusters[c].centroid[m] =
            clusters[c].centroid[m]/(double) clusters[c].listOfIndexes.size();

        total += clusters[c].centroid[m];
      }

    }
  }

  return clusters;
}

vector<Cluster> Cluster::clusterData(vector<vector<double>> data, int k, int maxIterations) {
    vector<vector<double>> centroids = getCentroids(data, k, maxIterations);
    vector<Cluster> clusters(centroids.size());

    for (int i = 0; i < data.size(); i++) {
      int minDistance = numeric_limits<double>::max();
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
      for(int d = 0; d < centroid.size(); ++d) {
        centroid[d] /= count;
      }
      centroids[j] = centroid;
    }
  }

  return centroids;
}
