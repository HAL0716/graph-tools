#pragma once

#include "Graph/Data.hpp"

namespace Graph {

class Analyzer {
public:
    Analyzer() = default;
    explicit Analyzer(const Data& data);

    void setData(const Data& data);

    int diameter();
    int radius();
    double avgPathLength();
    const std::vector<int>& eccentricity();
    double maxEigenvalue();

private:
    void calcDist();
    void calcEigen();

    const Data* g = nullptr;

    std::vector<int> ecc;
    bool eccCalced = false;

    double avgPL = -1;
    double maxEigen = -1;

    int diam = -1;
    int rad = -1;
    bool eigenCalced = false;
};

} // namespace Graph
