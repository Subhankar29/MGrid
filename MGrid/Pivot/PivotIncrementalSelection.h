#ifndef MGRID_PIVOTINCREMENTALSELECTION_H
#define MGRID_PIVOTINCREMENTALSELECTION_H

#include "../../MetricStructs/MetricObject.h"
#include "Pivot.h"
#include <iostream>
#include <vector>

using namespace std;

class PivotIncrementalSelection {
public:
    static vector<vector<double>> selectPivots(
            const vector<vector<double>> &metrics,
            int numberOfPivots,
            int numberOfRings,
            int sampleSize);

    static double vectorDistance(vector<double> v1, vector<double> v2);
};

#endif //MGRID_PIVOTINCREMENTALSELECTION_H
