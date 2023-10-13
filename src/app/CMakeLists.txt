include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(qsigviewer
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        filedialog.cpp
        filedialog.h
        filedialog.ui
        signaldata.cpp
        signaldata.h
)

target_link_libraries(qsigviewer
        Qt::Core
        Qt::Gui
        Qt::Widgets
        QCustomPlot
)

set_target_properties(qsigviewer PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
)
