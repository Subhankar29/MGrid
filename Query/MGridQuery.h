#ifndef MGRID_MGRIDQUERY_H
#define MGRID_MGRIDQUERY_H

#include "Query.h"
#include "../MetricStructs/MetricObject.h"
#include "../MGrid/MGrid/MGrid.h"
#include <iostream>
#include <vector>

using namespace std;

class MGridQuery: public Query {
public:
    explicit MGridQuery(MGrid  mGrid);
    vector<MetricObject> search(const QueryParameter& queryParameters) const override;
    void insert(const MetricObject& metricObject) const override;
    void deleteObject(const MetricObject& metricObject) const override;

private:
    vector<MetricObject> metrics;
    MGrid mGrid;
};

#endif //MGRID_MGRIDQUERY_H
