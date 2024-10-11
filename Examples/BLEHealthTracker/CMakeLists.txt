cmake_minimum_required(VERSION 3.16)

project(BLEHealthTracker VERSION 0.1 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set qt qml out put dir
set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml)

# set QT_IMPORT_PATH
set(QML_IMPORT_PATH
    ${QT_QML_OUTPUT_DIRECTORY}
    CACHE
    STRING ""
    FORCE
)


find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(MODULE_CACHEGEN_STATE
        NO_CACHEGEN
    )
endif()

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appBLEHealthTracker
    main.cpp
)

qt_add_qml_module(appBLEHealthTracker
    URI BLEHealthTracker
    VERSION 1.0
    ${MODULE_CACHEGEN_STATE}
    RESOURCE_PREFIX /
    QML_FILES
        Main.qml
        DataCard.qml
        Dashboard.qml
        SearchDevicePopup.qml
        CustomBLECentral.qml

    RESOURCES
        Res/blood-pressure-icon.svg
        Res/calories-vecteezy.svg
        Res/distance-svgrepo.svg
        Res/heart-rate-svgrepo.svg
        Res/leg-footprints-icon.svg
        Res/temperature.svg
        Res/bluetooth-icon.png
)

add_subdirectory(../../ ${CMAKE_BINARY_DIR}/qml/QuickBluetooth)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appBLEHealthTracker PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appBLEHealthTracker
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appBLEHealthTracker
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appBLEHealthTracker
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
