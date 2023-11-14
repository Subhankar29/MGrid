#include "PivotIncrementalSelection.h"
#include "Pivot.h"
#include "random"

using namespace std;

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
            // TODO: Pass the value by reference
            // Vector 1 -> 0.1, 0,2..
            // vector 2 -> 0.05, 0,06..
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
 * @param v1
 * @return
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
