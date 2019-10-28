cmake_minimum_required(VERSION 3.5)

project(diagnosticdaemonUI LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

set(resource_qrc resource.qrc)
qt5_add_resources(qrc_moc ${resource_qrc})


include_directories(~/Documents/icub-firmware-shared/eth/embot ~/Documents/icub-firmware-shared/eth/embobj/core/core ~/Documents/DiagnosticDaemon/src)

add_executable(diagnosticdaemonUI
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  msglisttablemodel.cpp
  msglisttablemodel.h
  paramtablemodel.cpp
  paramtablemodel.h
  ../src/RopParser.cpp
  ../src/RopParser.h
  ../src/pugixml.cpp
  ${qrc_moc}
)

target_link_libraries(diagnosticdaemonUI PRIVATE Qt5::Widgets Qt5::Network)
