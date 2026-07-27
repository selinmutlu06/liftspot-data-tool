cmake_minimum_required(VERSION 3.10)
project(liftspot_data_tool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(liftspot_data_tool
    src/main.cpp
    src/csv_utils.cpp
    src/geo_utils.cpp
    src/validator.cpp
    src/enrich.cpp
)
