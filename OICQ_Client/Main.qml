import QtQuick
import QtQuick.Controls
import QtQuick.Window
import "./components" as MyComponent
import oicqclient

FramelessWindow {
    id:rootWindow
    width: 325
    height: 450
    visible: true
    title: qsTr("OICQ")
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
    // Connections{
    //     target: loginContent

    //     function onLoginRequest() {
    //         loginContent.visible = false
    //         rootWindow.hide()
    //         timer.start()
    //     }
    // }

    // LoginContent{
    //     id:loginContent
    //     anchors.fill: parent
    //     visible: true
    // }

    // MainViewContent{
    //     id:mainViewContent
    //     anchors.fill: parent
    //     visible: false
    // }
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





