import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import "./components" as MyComponent
import NetWorks
import "./comman/NetChat.js" as NetChat

Item{
    signal loginRequest()
    property bool connectRes: false
    Rectangle{
        id:dyniBack
        // anchors.fill: parent
        width: 1600
        height: 2000
        color: "white"
        rotation: -20
        gradient: Gradient {
            GradientStop {
               position: 0.0
               color: "#5eb5be"
            }
            GradientStop {
               position: 0.3
               color: "#aef5fe"
            }
            GradientStop {
               position: 0.5
               color: "#939ab9"
            }
            GradientStop {
               position: 0.7
               color: "#7ef5fe"
            }
            GradientStop {
               position: 1.0
               color: "#636aa9"
            }
        }
        SequentialAnimation{
            loops: Animation.Infinite
            running: true
            ParallelAnimation {
                NumberAnimation {
                    target:dyniBack
                    property: "x"
                    from: -1300
                    to: -220
                    duration: 4600
                }
                NumberAnimation {
                    target: dyniBack
                    property: "y"
                    from: -1300
                    to: -220
                    duration: 4600
                }
            }
            ParallelAnimation {
                NumberAnimation {
                    target:dyniBack
                    property: "x"
                    from: -220
                    to: -1300
                    duration: 4600
                }
                NumberAnimation {
                    target: dyniBack
                    property: "y"
                    from: -220
                    to: -1300
                    duration: 4600
                }
            }
        }


    }


    Column{
        width: 200
        anchors.centerIn: parent
        spacing: 12
        MyComponent.HeadImage{
            width: 90
            height: 90
            headUrl: "qrc:/image/default_head.png"
            imageRadius: 45
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Rectangle{
            height: 1
            width: 20
            anchors.horizontalCenter: parent.horizontalCenter
            color: "transparent"
        }


        TextField {
            id: username
            width: 260
            height: 40
            text: "100001"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 18
            verticalAlignment: TextInput.AlignVCenter
            horizontalAlignment: TextInput.AlignHCenter
        }
        TextField {
            id: password
            width: 260
            height: 40
            text: "123456"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 18
            verticalAlignment: TextInput.AlignVCenter
            horizontalAlignment: TextInput.AlignHCenter
            echoMode: TextField.Password // 设置为密码输入模式
        }
        CheckBox {
            id: radioBtn
            text: "已阅读并同意服务协议和隐私保护指引"
            // checked: false
            checked: true
            font.pixelSize: 12
            x : -38
            indicator: Rectangle {    // 绘制圆圈
                id : icon
                implicitWidth: 18
                implicitHeight: 18
                x: parent.leftPadding
                y: parent.height / 2 - height / 2
                radius: height / 2
                border.color: radioBtn.checked ? "#0099ff" : "#888888"
                border.width: 1

                Rectangle {           // 绘制内圆圈
                    width: parent.width * 0.7
                    height: parent.height * 0.7
                    x: parent.width * 0.15
                    y: parent.height * 0.15
                    radius: height / 2
                    color: parent.border.color
                    visible: radioBtn.checked
                }
            }

            contentItem: Text {
                text: parent.text
                font: parent.font
                opacity: enabled ? 1.0 : 0.3
                color: "#656565"
                verticalAlignment: Text.AlignVCenter
                leftPadding: parent.indicator.width + 5
            }
            onCheckedChanged: {
                icon.border.color = checked ? "#0099ff" : "#888888"
            }
        }


        Button{
            id:loginBtn
            width: 260
            height: 40
            enabled: !(!radioBtn.checked || username.text == ""  || password.text == "")
            anchors.horizontalCenter: parent.horizontalCenter
            background: Rectangle{
                id:loginBtnBack
                color: enabled ? "#0099ff" : "#44ccff"
                opacity: enabled ? 1 : 0.75
            }
            contentItem: Text{
                id:loginBtnText
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: "登录"
                color: "white"
                font.pixelSize: 16
            }
            onDownChanged: {
                if(enabled && down){
                    loginBtnBack.color = Qt.lighter(loginBtnBack.color,0.9)
                    loginBtnText.color = Qt.lighter(loginBtnText.color, 0.9)
                }else{
                    loginBtnBack.color = Qt.lighter(loginBtnBack.color,1.1)
                    loginBtnText.color = Qt.lighter(loginBtnText.color, 1.1)
                }
            }
            onClicked: {
                // console.log("准备登录")
                //调用单例对象的方法
                if(!connectRes){
                    TcpSocket.connectToServer()
                    timer.restart()
                }else{
                    TcpSocket.packingMessage("{" + "\"userId\":" + username.text+ ","
                                             + "\"password\":" + "\"" + password.text + "\"" + "}",NetChat.MSG_TYPE.Login)  //登录
                }
            }
        }
    }

    //连接单例socket的信号
    Connections{
        target: TcpSocket
        function onConnected() {
            timer.stop()
            connectRes = true
            TcpSocket.packingMessage("{" + "\"userId\":" + username.text+ ","
                                     + "\"password\":" + "\"" + password.text + "\"" + "}",NetChat.MSG_TYPE.Login)  //登录
        }
        function onLoginReturnToQml(res){
            if(res === NetChat.RETURN.Success){
                console.log("Login.qml :: 登录成功")
                loginRequest()
            }
        }
    }

    MessageDialog{
        id:messageDialog
        title: qsTr("连接服务器失败，请检查你的网络")
        informativeText: "network error"
        buttons: MessageDialog.Ok
    }

    Timer{
        id:timer
        interval: 2000
        onTriggered: {
            if(!connectRes){
                messageDialog.open()
            }
        }
    }


}
