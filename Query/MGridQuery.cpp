#include "MGridQuery.h"

#include <utility>

MGridQuery::MGridQuery(MGrid mGrid) : mGrid(std::move(mGrid)) {}

void MGridQuery::insert(const MetricObject &metricObject) const {}

void MGridQuery::deleteObject(const MetricObject &metricObject) const {}

vector<MetricObject> MGridQuery::search(const QueryParameter &queryParameters) const {}
