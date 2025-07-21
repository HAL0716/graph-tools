#include "Graph/Analyzer.hpp"
#include <queue>
#include <Eigen/Eigenvalues>
#include <limits>
#include <numeric>

namespace Graph {

Analyzer::Analyzer(const Data& data) {
    setData(data);
}

void Analyzer::setData(const Data& data) {
    g = &data;
    ecc.clear();
    eccCalced = false;
    avgPL = -1;
    maxEigen = -1;
    diam = -1;
    rad = -1;
    eigenCalced = false;
}

struct BFSState {
    std::vector<int> dist;
    std::queue<int> q;

    BFSState(int n) : dist(n, -1) {}

    void push(int v, int d) {
        if (dist[v] != -1) return;
        dist[v] = d;
        q.push(v);
    }

    std::pair<int, int> front() const {
        int u = q.front();
        return {u, dist[u]};
    }

    void pop() {
        q.pop();
    }

    bool empty() const {
        return q.empty();
    }
};

void Analyzer::calcDist() {
    if (!g) throw std::runtime_error("Data is not set.");

    const auto& adj = g->getAdjList();
    const int n = static_cast<int>(adj.size());

    ecc.assign(n, 0);
    long long totalDistance = 0;
    long long reachablePairs = 0;

    for (int i = 0; i < n; ++i) {
        BFSState state(n);
        state.push(i, 0);

        while (!state.empty()) {
            auto [u, d] = state.front();
            state.pop();

            for (const auto& e : adj[u])
                state.push(e.dst, d + 1);
        }

        int eccentricity = 0;
        for (int d : state.dist) {
            if (d > 0) {
                totalDistance += d;
                ++reachablePairs;
                eccentricity = std::max(eccentricity, d);
            }
        }
        ecc[i] = eccentricity;
    }

    auto validMin = [](const std::vector<int>& v) -> int {
        int minVal = std::numeric_limits<int>::max();
        for (int x : v) {
            if (x > 0 && x < minVal)
                minVal = x;
        }
        return (minVal == std::numeric_limits<int>::max()) ? 0 : minVal;
    };

    avgPL = reachablePairs > 0 ? static_cast<double>(totalDistance) / reachablePairs : 0.0;
    diam = *std::max_element(ecc.begin(), ecc.end());
    rad = validMin(ecc);

    eccCalced = true;
}

const std::vector<int>& Analyzer::eccentricity() {
    if (!eccCalced) calcDist();
    return ecc;
}

int Analyzer::diameter() {
    if (!eccCalced) calcDist();
    return diam;
}

int Analyzer::radius() {
    if (!eccCalced) calcDist();
    return rad;
}

double Analyzer::avgPathLength() {
    if (!eccCalced) calcDist();
    return avgPL;
}

void Analyzer::calcEigen() {
    const auto& A = g->getAdjMatrix();
    Eigen::EigenSolver<Eigen::MatrixXd> solver(g->getAdjMatrix());
    maxEigen = solver.eigenvalues().real().maxCoeff();
    eigenCalced = true;
}

double Analyzer::maxEigenvalue() {
    if (!eigenCalced) calcEigen();
    return maxEigen;
}

} // namespace Graph