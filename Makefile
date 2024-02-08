# Project name
PROJECT = main

# Source files
SRC = $(PROJECT).cpp \
      Query/MGridQuery.cpp \
      MGrid/Pivot/PivotIncrementalSelection.cpp \
      MGrid/Cluster/Cluster.cpp \
      MGrid/Pivot/Pivot.cpp \
      MGrid/MGrid/MGrid.cpp \
      GenerateData/DataSetGenerator.cpp

# Header files 
HEADERS = Query/Query.h \
		 MetricStructs/MetricObject.h \
		 Query/QueryParameter.h \
		 Query/MGridQuery.h \
		 MGrid/Pivot/PivotIncrementalSelection.h \
		 MGrid/Cluster/Cluster.h \
		 MGrid/Pivot/Pivot.h \
		 MGrid/MGrid/MGrid.h \
		 GenerateData/DataSetGenerator.h
         

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Target
$(PROJECT): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(PROJECT) $(SRC)

.PHONY: clean
clean:
	rm -f $(PROJECT)

.PHONY: run
run: $(PROJECT)
	./$(PROJECT)
