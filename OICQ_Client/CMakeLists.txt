cmake_minimum_required(VERSION 3.27)

project(
  oicqclient
  VERSION 0.1
  LANGUAGES CXX)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Network)

qt_standard_project_setup(REQUIRES 6.4)

qt_add_executable(appoicqclient)

qt_add_qml_module(
  appoicqclient
  URI
  oicqclient
  VERSION
  1.0
  QML_FILES # .qml
  Main.qml
  SideMenuBar.qml
  components/IconButton.qml
  components/BackIconButton.qml
  components/HeadImage.qml
  friendChat/RecentFriendList.qml
  friendChat/MessageList.qml
  components/SearchBox.qml
  friendChat/RecentFriendListDelegate.qml
  friendChat/FriendChatWindow.qml
  friendChat/MessageDelegate.qml
  friendChat/MessageInputField.qml
  comman/NetChat.js
  components/HeadUpLoadPopup.qml
  components/ImageViewer.qml
  components/ScreenPrint.qml
  friendChat/FriendsGroupMenu.qml
  friendChat/FriendsGroupPopup.qml
  friendChat/FriendAddWindow.qml
  friendChat/ChatFile.qml
  components/WindowBorder.qml
  Login.qml
  components/PictureViewer.qml
  components/RightPopUpMenu.qml
  friendChat/MemeView.qml
  components/RoundCheckBox.qml
  components/RepeatLogin.qml
  components/LoginFail.qml
  friendChat/SendRestriction.qml
  Dialogs.qml
  Actions.qml
  components/ThreeButtons.qml
  MainView.qml
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
  controller.h
  controller.cpp
  RESOURCES # .qrc
  rec.qrc)

target_compile_features(appoicqclient PRIVATE cxx_std_20)

target_include_directories(appoicqclient PRIVATE . ./network)

set_target_properties(
  appoicqclient
  PROPERTIES AUTORCC TRUE
             MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
             MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
             MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
             MACOSX_BUNDLE TRUE
             WIN32_EXECUTABLE TRUE)

target_link_libraries(appoicqclient PRIVATE Qt6::Quick Qt6::Network)

include(GNUInstallDirs)
install(
  TARGETS appoicqclient
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
