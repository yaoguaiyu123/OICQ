cmake_minimum_required(VERSION 3.27)

project(
  qqClient
  VERSION 0.1
  LANGUAGES CXX)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Network)

qt_standard_project_setup(REQUIRES 6.4)

qt_add_executable(appqqV1)

qt_add_qml_module(
  appqqV1
  URI
  qqClient
  VERSION
  1.0
  QML_FILES # .qml
  Main.qml
  MainView.qml
  SideMenuBar.qml
  components/IconButton.qml
  components/BackIconButton.qml
  components/HeadImage.qml
  friendChat/RecentFriendList.qml
  friendChat/MessageList.qml
  components/SearchBox.qml
  friendChat/RecentFriendListDelegate.qml
  comman/Comman.js
  friendChat/FriendChatWindow.qml
  friendChat/MessageDelegate.qml
  friendChat/MessageInputField.qml
  Login.qml
  comman/NetChat.js
  components/HeadUpLoadPopup.qml
  components/ImageViewer.qml
  components/ScreenPrint.qml
  friendChat/FriendsGroupMenu.qml
  friendChat/FriendsGroupPopup.qml
  friendChat/FriendAddWindow.qml
  friendChat/ChatFile.qml
  components/WindowBorder.qml
  SOURCES # .h && .cpp
  main.cpp
  network/postsend.h
  network/postsend.cpp
  network/tcpsocket.h
  network/tcpsocket.cpp
  network/fileclient.h
  network/fileclient.cpp
  global.h
  friendmodel.h
  friendmodel.cpp
  frienddata.h
  frienddata.cpp
  messagemodel.h
  messagemodel.cpp
  global.cpp
  textdocumenthandler.h
  textdocumenthandler.cpp
  friendrequestmodel.h
  friendrequestmodel.cpp
  framelesswindow.h
  framelesswindow.cpp
  screencapture.h
  screencapture.cpp
  RESOURCES # .qrc
  rec.qrc
  QML_FILES components/RightPopUpMenu.qml)

target_compile_features(appqqV1 PRIVATE cxx_std_20)

set_target_properties(
  appqqV1
  PROPERTIES AUTORCC TRUE
             MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
             MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
             MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
             MACOSX_BUNDLE TRUE
             WIN32_EXECUTABLE TRUE)

target_link_libraries(appqqV1 PRIVATE Qt6::Quick Qt6::Network)

include(GNUInstallDirs)
install(
  TARGETS appqqV1
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# qt_add_executable(qqClient)  //Creates and finalizes an application target of
# a platform-specific type. This command performs the following tasks: 1. Create
# a CMake target of the appropriate type for the target platform. 2. Link the
# target to the Qt::Core library. 3. Handle finalization of the CMake target.

# qt_standard_project_setup()     #set(CMAKE_AUTOUIC ON)  set(CMAKE_AUTOMOC ON)

# @if %{HasTranslation} find_package(Qt6 REQUIRED COMPONENTS Core Quick
# LinguistTools) @else find_package(Qt6 REQUIRED COMPONENTS Core Quick) @endif

# target_sources(appqqV1 PRIVATE main.cpp qml.qrc @if %{HasTranslation}
# %{TsFileName} @endif
#
# PUBLIC
#
# )

# qt_import_qml_plugins(qqClient)

# cmake_minimum_required(VERSION 3.16)
#
# project(qqClient VERSION 0.1 LANGUAGES CXX)
#
# @if !true set(CMAKE_AUTOMOC ON) @endif set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
# find_package(Qt6 6.6 REQUIRED COMPONENTS Quick)
#
# @if true @if true qt_standard_project_setup(REQUIRES 6.5) @else
# qt_standard_project_setup() @endif @endif
#
# qt_add_executable(appqqV1 main.cpp )
#
# qt_add_qml_module(appqqV1 URI qqClient VERSION 1.0 QML_FILES Main.qml )
#
# # Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1. #
# If you are developing for iOS or macOS you should consider setting an #
# explicit, fixed bundle identifier manually though.
# set_target_properties(appqqV1 PROPERTIES #    MACOSX_BUNDLE_GUI_IDENTIFIER
# com.example.appqqV1 MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
# MACOSX_BUNDLE_SHORT_VERSION_STRING
# ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE TRUE
# WIN32_EXECUTABLE TRUE )
#
# target_link_libraries(appqqV1 PRIVATE Qt6::Quick )
#
# include(GNUInstallDirs) install(TARGETS appqqV1 BUNDLE DESTINATION . LIBRARY
# DESTINATION ${CMAKE_INSTALL_LIBDIR} RUNTIME DESTINATION
# ${CMAKE_INSTALL_BINDIR} )
