#include "Pivot.h"
#include <utility>

Pivot::Pivot(string id, MetricObject metricObject, const vector<double>& ringIndexes)
: id(std::move(id)), metricObject(metricObject),  ringIndexes(ringIndexes) {}

vector<double> Pivot::getRingIndexes() {
    return ringIndexes;
}

double Pivot::getRingDistance() const {
    return ringDistance;
}
