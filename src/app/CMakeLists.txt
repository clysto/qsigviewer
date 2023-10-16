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
        infinitydial.cpp
        infinitydial.h
        timeticker.cpp
        timeticker.h
        utils.cpp
        utils.h
)

target_link_libraries(qsigviewer
        Qt::Core
        Qt::Gui
        Qt::Widgets
        QCustomPlot
)

target_compile_definitions(qsigviewer PRIVATE QCUSTOMPLOT_USE_LIBRARY)

if (CMAKE_BUILD_TYPE MATCHES "Release")
    set_target_properties(qsigviewer PROPERTIES
            WIN32_EXECUTABLE ON
            MACOSX_BUNDLE ON
    )
endif ()


if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "$<TARGET_FILE_DIR:QCustomPlot>/libQCustomPlot.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
endif ()
