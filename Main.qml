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
// import FluentUI

//大体的布局就是左边一个菜单栏
//中间上面一个搜索栏
//中间下面一个ListView
//右边上面一个系统按钮组
//右边中间是ListView实现的聊天窗口
//右边下面是输入窗口

FramelessWindow {
    id:rootWindow
    width: 325
    height: 450
    // width: 1100
    // height: 710
    visible: true
    title: qsTr("qqClient")
    // minimumHeight: 530
    // minimumWidth: 840

    Loader {
        id: loader
        source: "Login.qml"
        anchors.fill: parent
        // TODO qml编译器报错无法检测到信号，但是实际运行还是可以连接到

        Connections{
            target: loader.item

            function onLoginRequest() {
                rootWindow.visible = false
                timer.start()
                toMainViewAni.start()
                loader.source = "MainView.qml"
            }
        }
        // 加上动画之后误打误撞解决了bug
        ParallelAnimation{
            id:toMainViewAni
            running: false
            NumberAnimation{
                target: rootWindow
                properties: "x"
                to: (Screen.width - 1100) / 2
                duration: 200
                easing.type: Easing.OutQuad
            }
            NumberAnimation{
                target: rootWindow
                properties: "y"
                to: (Screen.height - 710) / 2
                duration: 200
                easing.type: Easing.OutQuad
            }
        }
    }

    Timer{
        id:timer
        interval: 300
        onTriggered: {
            rootWindow.visible = true
            console.log(rootWindow.width,rootWindow.height)
            rootWindow.width = 1100
            rootWindow.height = 710
        }
    }



}


// FramelessWindow {
//     id: root
//     visible: true
//     width: 640
//     height: 480
//     minimumWidth: 480
//     minimumHeight: 320
//     color: "white"
//     title: qsTr("Hello World")

// }
