cmake_minimum_required(VERSION 3.26)
project(qsigviewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        PrintSupport
        REQUIRED
)

add_subdirectory(libs/qcustomplot)
add_subdirectory(src/app)
