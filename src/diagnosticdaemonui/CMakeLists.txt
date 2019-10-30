cmake_minimum_required(VERSION 3.5)

project(diagnosticdaemonui LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

set(resource_qrc resource.qrc)
qt5_add_resources(qrc_moc ${resource_qrc})


include_directories(../diagnosticdaemon ../../../icub-firmware-shared/eth/embot ../../../icub-firmware-shared/eth/embobj/core/core)

add_executable(diagnosticdaemonui
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  msglisttablemodel.cpp
  msglisttablemodel.h
  settingstablemodel.h
  settingstablemodel.cpp
  paramtablemodel.cpp
  paramtablemodel.h
  ../diagnosticdaemon/RopParser.cpp
  ../diagnosticdaemon/RopParser.h
  ../diagnosticdaemon/pugixml.cpp
  ${qrc_moc}
  app.rc
)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/uiconfig.xml DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

target_link_libraries(diagnosticdaemonui PRIVATE Qt5::Widgets Qt5::Network)
