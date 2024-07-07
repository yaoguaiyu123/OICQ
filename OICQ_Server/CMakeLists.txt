cmake_minimum_required(VERSION 3.27)

project(
  oicqserver
  VERSION 0.1
  LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Sql)
qt_standard_project_setup()

qt_add_executable(
  oicqserver
  widget.h
  widget.cpp
  main.cpp
  tcpserver.h
  tcpserver.cpp
  clienthandler.h
  clienthandler.cpp
  global.h
  database/dbmanager.h
  database/dbmanager.cpp
  global.cpp
  fileclienthandler.h
  fileclienthandler.cpp
  fileserver.h
  fileserver.cpp
  rec.qrc
  chatmember.h chatmember.cpp)

target_compile_features(oicqserver PRIVATE cxx_std_20)

target_link_libraries(
  oicqserver
  PRIVATE Qt6::Widgets
  PRIVATE Qt6::Network
  PRIVATE Qt6::Sql)

set_target_properties(
  oicqserver
  PROPERTIES AUTORCC TRUE
             MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
             MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
             MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
             MACOSX_BUNDLE TRUE
             WIN32_EXECUTABLE TRUE)

include(GNUInstallDirs)
install(
  TARGETS oicqserver
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

qt_finalize_executable(oicqserver)
