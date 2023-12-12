cmake_minimum_required(VERSION 3.24)
project(Wezly)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Wezly
        helpers.hpp
        nodes.hpp
        package.cpp
        package.hpp
        storage_types.cpp
        storage_types.hpp
        types.hpp nodes.cpp helpers.cpp
	main.cpp)
