#ifndef MGRID_PIVOT_H
#define MGRID_PIVOT_H

#include "../../MetricStructs/MetricObject.h"
#include <string>
#include <vector>

using namespace std;

class Pivot {
private:
    string id;
    MetricObject metricObject;
    vector<double> ringIndexes;
    double ringDistance; // todo- we can have non-uniform rings in the grid in future

public:
    Pivot(string id, MetricObject metricObject, const vector<double>& ringIndexes);
    vector<double> getRingIndexes();
    double getRingDistance() const;
};

#endif //MGRID_PIVOT_H
