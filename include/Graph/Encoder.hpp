#pragma once

#include <string>
#include <unordered_map>
#include <type_traits>
#include "Utils/Func.hpp"

namespace Graph {

class Encoder {
public:
    // 文字列シーケンスをエンコード
    template <typename Iterable>
    int encode(const Iterable& key) {
        return encode(Utils::Func::join(key));
    }

    // 単一の文字列をエンコード
    int encode(const std::string& key) {
        auto [it, inserted] = table_.emplace(key, nextId_);
        if (inserted) ++nextId_;
        return it->second;
    }

    // 登録済みキー数
    int size() const noexcept {
        return static_cast<int>(table_.size());
    }

    // 状態のクリア
    void clear() {
        table_.clear();
        nextId_ = 0;
    }

private:
    std::unordered_map<std::string, int> table_;
    int nextId_ = 0;                              // 次に割り当てるID
};

} // namespace Graph
