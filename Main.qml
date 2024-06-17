// Date:2024/4/30
// Author:妖怪鱼
// Introduction:qqClient
// 使用QML实现的一个仿QQ,先实现基本功能和界面
import QtQuick
import QtQuick.Controls
import QtQuick.Window
import "./components" as MyComponent
import CustomModels
import CustomWindows

FramelessWindow {
    id:rootWindow
    width: 325
    height: 450
    visible: true
    title: qsTr("qqClient")
    x: (Screen.width - width) / 2
    y: (Screen.height - height) / 2

    Connections{
        target: loginItem

        function onLoginRequest() {
            loginItem.visible = false
            rootWindow.hide()
            timer.start()
        }
    }



    Login{
        id:loginItem
        anchors.fill: parent
        visible: true
    }

    MainView{
        id: chatItem
        anchors.fill: parent
        visible: false
    }

    Timer{
        id:timer
        interval: 300
        onTriggered: {
            rootWindow.show()
            rootWindow.width = 1080
            rootWindow.height = 695
            rootWindow.x = (Screen.width - width) / 2
            rootWindow.y = (Screen.height - height) / 2
            rootWindow.minimumHeight = 500
            rootWindow.minimumWidth = 800
            chatItem.visible = true
        }
    }

}


