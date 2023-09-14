cmake_minimum_required(VERSION 3.26)
project(pwm)

set(CMAKE_CXX_STANDARD 23)

add_executable(pwm main.cpp
        accountHandler.h
        accountHandler.cpp
        modules/utils/aes256.hpp
        modules/utils/aes256.cpp
)
