include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# find fftw3
find_package(FFTW3 REQUIRED)
include_directories(${FFTW3_INCLUDE_DIR})

if (CMAKE_BUILD_TYPE MATCHES "Release")
    if (APPLE)
        set(MACOSX_BUNDLE_ICON_FILE qsigviewer.icns)
        set(MACOSX_BUNDLE_ICON_PATH ${CMAKE_CURRENT_LIST_DIR}/resources/qsigviewer.icns)
        set_source_files_properties(${MACOSX_BUNDLE_ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    endif ()
endif ()

add_executable(qsigviewer
        MACOSX_BUNDLE
        ${MACOSX_BUNDLE_ICON_PATH}
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

if (CMAKE_BUILD_TYPE MATCHES "Release")
    set_target_properties(qsigviewer PROPERTIES
            WIN32_EXECUTABLE ON
            MACOSX_BUNDLE ON
    )
endif ()

target_link_libraries(qsigviewer
        Qt::Core
        Qt::Gui
        Qt::Widgets
        QCustomPlot
)

target_compile_definitions(qsigviewer PRIVATE QCUSTOMPLOT_USE_LIBRARY)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "$<TARGET_FILE_DIR:QCustomPlot>/libQCustomPlot.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
endif ()
