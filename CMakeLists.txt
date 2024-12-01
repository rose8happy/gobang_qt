cmake_minimum_required(VERSION 3.30)

project(gobang)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 COMPONENTS Quick REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTORCC ON)  # 自动处理资源文件
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 14)

add_executable(gobang
    main.cpp
        resources/gobang.qrc
        GobangBoard.h
        GobangBoard.cpp
        GobangRuleChecker.h
        GobangRuleChecker.cpp
        GobangAI.cpp
        GobangAI.h)

# 链接Qt5库
target_link_libraries(gobang Qt5::Widgets)
target_link_libraries(gobang Qt5::Quick)