if (WIN32)
    set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resources/qsigviewer.rc")
elseif (APPLE)
    set(MACOSX_BUNDLE_ICON_FILE qsigviewer.icns)
    set(MACOSX_BUNDLE_ICON_PATH ${CMAKE_CURRENT_LIST_DIR}/resources/qsigviewer.icns)
    set_source_files_properties(${MACOSX_BUNDLE_ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set(app_icon_resource_windows)
endif ()

set(SOURCES
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
        spectrumwin.cpp
        spectrumwin.h
        spectrumwin.ui
        ${app_icon_resource_windows}
)

if (APPLE)
    add_executable(qsigviewer MACOSX_BUNDLE ${MACOSX_BUNDLE_ICON_PATH} ${SOURCES})
else ()
    add_executable(qsigviewer ${SOURCES})
endif ()


if (WIN32)
    set(FFTW3_INCLUDE_DIRS "${FFTW3_DIR}")
    set(FFTW3_LIBRARIES "${FFTW3_DIR}/libfftw3-3.dll")
elseif (APPLE)
    set(FFTW3_LIBRARIES /usr/local/opt/fftw/lib/libfftw3.dylib)
    set(FFTW3_LIBRARY_DIRS /usr/local/opt/fftw/lib)
    set(FFTW3_INCLUDE_DIRS /usr/local/opt/fftw/include)
endif ()

target_include_directories(qsigviewer PRIVATE ${FFTW3_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(qsigviewer
        Qt::Core
        Qt::Gui
        Qt::Widgets
        QCustomPlot
        ${FFTW3_LIBRARIES}
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


if (CMAKE_BUILD_TYPE MATCHES Release)
    if (WIN32)
        set(DEPLOY_CMD windeployqt)
    elseif (APPLE)
        get_filename_component(QT_MAC_QT_ROOT "${Qt6Core_DIR}/../../.." ABSOLUTE)
        find_program(QT_MAC_DEPLOY_APP macdeployqt PATHS ${QT_MAC_QT_ROOT}/bin)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${QT_MAC_DEPLOY_APP} qsigviewer.app "-dmg"
        )
    endif ()
endif ()