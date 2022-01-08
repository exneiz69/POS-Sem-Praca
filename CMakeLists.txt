cmake_minimum_required(VERSION 3.7)
project(Client)

set(CMAKE_CXX_STANDARD 20)

add_executable(Client main.cpp Client.cpp Client.h data.h Gui.cpp Gui.h)
