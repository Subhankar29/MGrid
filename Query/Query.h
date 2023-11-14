#ifndef MGRID_QUERY_H
#define MGRID_QUERY_H

#include "QueryParameter.h"
#include "../MetricStructs/MetricObject.h"

class Query {
public:
    virtual vector<MetricObject> search(const QueryParameter& queryParameters) const = 0;
    virtual void insert(const MetricObject& metricObject) const = 0;
    virtual void deleteObject(const MetricObject& metricObject) const = 0;
};

#endif //MGRID_QUERY_H
