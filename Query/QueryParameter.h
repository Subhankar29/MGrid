#ifndef MGRID_QUERYPARAMETER_H
#define MGRID_QUERYPARAMETER_H

#include <utility>

#include "../MetricStructs/MetricObject.h"
#include "../MGrid/Pivot/Pivot.h"
#include "../MGrid/Cluster/Cluster.h"

struct QueryParameter {
    MetricObject queryObject;
    vector<Pivot> pivots;
    vector<Cluster> clusters;
    float radius; // to-do

    QueryParameter(MetricObject queryObject, vector<Pivot> pivots, vector<Cluster> clusters, float radius):
    queryObject(queryObject), pivots(std::move(pivots)), clusters(std::move(clusters)), radius(radius){

    }


};

#endif //MGRID_QUERYPARAMETER_H
