#include "Graph/Analyzer.hpp"
#include <Eigen/Eigenvalues>
#include <queue>
#include <limits>

namespace Graph {

double Analyzer::maxEigenvalue(const Data& g) {
    Eigen::EigenSolver<Eigen::MatrixXd> solver(g.getAdjMatrix());
    return solver.eigenvalues().real().maxCoeff();
}

int Analyzer::diameter(const Data& g) {
    const auto& mat = g.getAdjMatrix();
    int n = mat.rows();
    int maxDist = 0;

    for (int i = 0; i < n; ++i) {
        std::vector<int> dist(n, -1);
        std::queue<int> q;
        q.push(i);
        dist[i] = 0;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v = 0; v < n; ++v) {
                if (mat(u, v) > 0 && dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }

        for (int d : dist)
            if (d > maxDist) maxDist = d;
    }

    return maxDist;
}

double Analyzer::avgPathLength(const Data& g) {
    const auto& mat = g.getAdjMatrix();
    int n = mat.rows();
    int total = 0, count = 0;

    for (int i = 0; i < n; ++i) {
        std::vector<int> dist(n, -1);
        std::queue<int> q;
        q.push(i);
        dist[i] = 0;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v = 0; v < n; ++v) {
                if (mat(u, v) > 0 && dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }

        for (int j = 0; j < n; ++j)
            if (j != i && dist[j] > 0) {
                total += dist[j];
                count++;
            }
    }

    return count > 0 ? static_cast<double>(total) / count : 0.0;
}

} // namespace Graph