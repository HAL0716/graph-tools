#pragma once

#include <unordered_map>
#include <string>
#include <sstream>
#include <iterator>
#include "Graph/Data.hpp"
#include "Utils/Func.hpp"

namespace Graph {

class Transform {
public:
    static Graph::Data delSinkNodes(const Graph::Data& data, bool isRecursive = true);
    static Graph::Data mergeByMoore(const Graph::Data& data);

private:
    class Encoder {
    public:
        template <typename Iterable>
        int encode(const Iterable& key) {
            return encode(Utils::Func::join(key));
        }

        int encode(const std::string& key);

        int size() const;
        void clear();

    private:
        std::unordered_map<std::string, int> table_;
        int nextId_ = 0;
    };
};

} // namespace Graph
