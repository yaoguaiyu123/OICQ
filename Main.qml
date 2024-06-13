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
            rootWindow.hide()
            timer.start()
        }
    }

    Login{
        id:loginItem
        anchors.fill: parent
    }

    FramelessWindow{
        id:workWindow
        width: 1080
        height: 705
        MainView{
            anchors.fill: parent
        }
    }

    Timer{
        id:timer
        interval: 300
        onTriggered: {
            workWindow.show()
        }
    }

}


