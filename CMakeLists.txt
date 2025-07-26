cmake_minimum_required(VERSION 3.10)
project(Graph-Tools)

# === コンパイラ設定 ===
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === インクルードディレクトリ ===
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    /opt/homebrew/include/eigen3
)

# === Catch2 を FetchContent で取得 ===
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.8.1
)
FetchContent_MakeAvailable(Catch2)

# === Spectra を FetchContent で取得 ===
FetchContent_Declare(
  Spectra
  GIT_REPOSITORY https://github.com/yixuan/spectra.git
  GIT_TAG master
)
FetchContent_MakeAvailable(Spectra)

# Spectra の include ディレクトリを追加
include_directories(${spectra_SOURCE_DIR}/include)

# === メイン実行ファイル（src/main.cpp） ===
add_executable(main src/main.cpp)

# 他のソースをオブジェクトライブラリとしてビルド
file(GLOB_RECURSE OTHER_SRC_FILES src/*.cpp)
list(REMOVE_ITEM OTHER_SRC_FILES ${CMAKE_SOURCE_DIR}/src/main.cpp)
add_library(objects OBJECT ${OTHER_SRC_FILES})

# メインにオブジェクトライブラリをリンク
target_link_libraries(main PRIVATE $<TARGET_OBJECTS:objects>)

# 実行ファイルの出力先を build ディレクトリに設定
set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# === Catch2 テスト用ターゲット ===
file(GLOB_RECURSE TEST_SRC_FILES tests/*.cpp)

enable_testing()

add_executable(tests ${TEST_SRC_FILES})
target_link_libraries(tests
    PRIVATE
        $<TARGET_OBJECTS:objects>
        Catch2::Catch2WithMain
)

# CTest + Catch2 の統合
include(CTest)
include(Catch)
catch_discover_tests(tests)
