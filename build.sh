#!/bin/bash

# エラーによる強制終了
set -e  

# ビルドディレクトリの作成と移動
mkdir -p build && cd build

# CMake の構成とビルド
cmake .. && cmake --build .

# Catch2 を使ったテストの実行（ctest 経由）
ctest --output-on-failure