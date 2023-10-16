add_library(QCustomPlot SHARED
        qcustomplot.cpp
        qcustomplot.h
)

target_link_libraries(QCustomPlot Qt6::Widgets Qt6::PrintSupport)

target_compile_definitions(QCustomPlot PRIVATE QCUSTOMPLOT_COMPILE_LIBRARY)

target_include_directories(QCustomPlot
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
