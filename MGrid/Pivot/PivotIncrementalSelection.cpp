#include "PivotIncrementalSelection.h"
#include "Pivot.h"
#include "random"

using namespace std;

/**
 * Create Pivot on the dataset based on incremental Pivot Selection Algorithm
 * @param metrics represents the dataset
 * @param numberOfPivots to be created
 * @param numberOfRings to be formed
 * @param sampleSize represents the smaller portion fo the dataset
 * @return a list of pivots
 */
vector<vector<double>> PivotIncrementalSelection::selectPivots(
        const vector<vector<double>>& metrics,
        int numberOfPivots,
        int numberOfRings,
        int sampleSize) {
    vector<vector<double>> selectedPivots(numberOfPivots);

    random_device randomDevice;

    mt19937 gen(randomDevice());

    // Select first pivot randomly
    uniform_int_distribution<> dist(0, metrics.size() - 1);
    int index = dist(gen);

    selectedPivots[0] = metrics[index];

    // Select subsequent pivots incrementally
    for (int i = 1; i < numberOfPivots; i++) {
        vector<double> deltas;

        for (int j = 0; j < sampleSize; j++) {

            // Randomly selecting the next pairs
            int index1 = dist(gen);
            int index2 = dist(gen);

            // Get the distance between the 2 indexes
            double delta = vectorDistance(metrics[index2], metrics[index1]);

            deltas.push_back(delta);
        }

        // Find the pivot from candidates that maximizes delta
        double maxDelta = 0;
        int maxIndex;

        for (int c = 0; c < sampleSize; c++) {
            index = dist(gen);

            double pivotDelta = 0;

            for (double delta : deltas) {
                pivotDelta += abs(delta - vectorDistance(metrics[index], selectedPivots[i - 1]));
            }

            if (pivotDelta > maxDelta) {
                maxDelta = pivotDelta;
                maxIndex = index;
            }
        }

        selectedPivots[i] = metrics[maxIndex];
    }

    return selectedPivots;
}

/**
 * Function to get the Euclidean distance between the 2 points
 * @param v1 point 1
 * @param v2 point 2
 * @return euclidian distance between 2 points
 */
double PivotIncrementalSelection::vectorDistance(vector<double> v1, vector<double> v2) {
    if (v1.size() != v2.size()) {
        throw "Vectors must be of the same size";
    }

    double distance = 0;

    for (int i = 0; i < v1.size(); i++) {
        double difference = v1[i] - v2[i];
        distance += difference * difference;
    }

    return sqrt(distance);
}
