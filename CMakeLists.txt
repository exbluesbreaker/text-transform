cmake_minimum_required (VERSION 3.8)

project ("TextTransform")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable (TextTransform "main.cpp"  "WordList.cpp" "WordList.h" "Graph.h" "Graph.cpp")
