//
// Created by subhankar on 10/10/2023.
//

#ifndef MGRID_QUERY_H
#define MGRID_QUERY_H

#include <rpcndr.h>
#include "../MetricObject/MetricObject.h"
#include "QueryParameter.h"


class Query {
public:
    virtual void search(const QueryParameter& queryParameters) const = 0;
    virtual void insert(const metricObject& metricObject) const = 0;
    virtual void deleteObject(const metricObject& metricObject) const = 0;
};

#endif //MGRID_QUERY_H
