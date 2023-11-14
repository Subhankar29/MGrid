//
// Created by subha on 10/10/2023.
//

#ifndef MGRID_PIVOT_H
#define MGRID_PIVOT_H
#include "../../MetricObject/MetricObject.h"

class Pivot {
    virtual double distance(metricObject metricObject) const = 0;
};


#endif //MGRID_PIVOT_H
