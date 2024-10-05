cmake_minimum_required(VERSION 3.16)

project(QuickBluetooth VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Core Bluetooth)

#
# Set up project properties
#

#
# Intialize the project
#

qt_add_qml_module(appQuickBluetooth
    URI QuickBluetooth
    VERSION 1.0
    SOURCES
        Src/BluetoothController.h
        Src/BluetoothController.cpp
        Src/BluetoothDeviceInfo.cpp 
        Src/BluetoothDeviceInfo.h 
        Src/BluetoothDiscovery.cpp 
        Src/BluetoothDiscovery.h
        Src/BLEPeripheral.cpp 
        Src/BLEPeripheral.h
)

#
# Target related properties and definitions
#

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appQuickBluetooth PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${MAIN_TARGET_NAME}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_include_directories(appQuickBluetooth PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Src
)

target_link_libraries(appQuickBluetooth PRIVATE
    Qt6::Core
    Qt6::Bluetooth
)

include(GNUInstallDirs)
install(TARGETS appQuickBluetooth
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
