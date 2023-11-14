cmake_minimum_required(VERSION 3.26)
project(MGrid)

set(CMAKE_CXX_STANDARD 17)

add_executable(MGrid main.cpp
        Query/Query.h
        MetricStructs/MetricObject.h
        Query/QueryParameter.h
        Query/MGridQuery.h
        Query/MGridQuery.cpp
        MGrid/Pivot/PivotIncrementalSelection.cpp
        MGrid/Pivot/PivotIncrementalSelection.h
        MGrid/Cluster/Cluster.cpp
        MGrid/Cluster/Cluster.h
        MGrid/Pivot/Pivot.cpp
        MGrid/Pivot/Pivot.h
        MGrid/MGrid/MGrid.cpp
        MGrid/MGrid/MGrid.h
        GenerateData/DataSetGenerator.cpp
        GenerateData/DataSetGenerator.h)
