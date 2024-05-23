#include "Pivot.h"
#include <utility>

/**
 * Class represent the Pivot Object
 * @param id is unique to identify the pivot
 * @param metricObject represent the individual pivot
 * @param ringIndexes stores the value of pivot
 */
Pivot::Pivot(string id, MetricObject metricObject, const vector<double>& ringIndexes)
: id(std::move(id)), metricObject(metricObject),  ringIndexes(ringIndexes) {}

/**
 * @return the pivot data point
 */
vector<double> Pivot::getRingIndexes() {
    return ringIndexes;
}

/**
 *
 * @return the distance of the closest ring
 */
double Pivot::getRingDistance() const {
    return ringDistance;
}
