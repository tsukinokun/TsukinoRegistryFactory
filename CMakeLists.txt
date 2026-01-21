cmake_minimum_required(VERSION 3.10)
project(TsukinoRegistryFactory LANGUAGES CXX)

# ヘッダーオンリーライブラリとして定義
add_library(TsukinoRegistryFactory INTERFACE)

# include ディレクトリを公開
target_include_directories(TsukinoRegistryFactory INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# C++17 を要求
target_compile_features(TsukinoRegistryFactory INTERFACE cxx_std_17)

# example をビルドしたい場合（任意）
option(TSF_BUILD_EXAMPLE "Build example program" ON)

if(TSF_BUILD_EXAMPLE)
    add_executable(tsf_example example/example.cpp)
    target_link_libraries(tsf_example PRIVATE TsukinoRegistryFactory)
endif()
