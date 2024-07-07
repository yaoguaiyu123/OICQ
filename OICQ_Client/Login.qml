import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import oicqclient
import "./components" as MyComponent
import "./comman/NetChat.js" as NetChat

Item{
    property alias allDialogs: dialogs

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
               color: "#321d28"
            }
            GradientStop {
               position: 0.2
               color: "#19194b"
            }
            GradientStop {
               position: 0.5
               color: "#2e1934"
            }
            GradientStop {
               position: 0.7
               color: "#321d28"
            }
            GradientStop {
               position: 1.0
               color: "#2e1a30"
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

    MyComponent.BackIconButton{
        anchors.right: parent.right
        anchors.top: parent.top
        imagePath: "qrc:/icon/close.png"
        imageHoverPath: "qrc:/icon/close_w.png"
        backColor: "#00000000"
        backHoverColor: "#cc4444"
        onClicked: {
            Qt.quit()  //关闭应用
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
            font.pixelSize: 14
            verticalAlignment: TextInput.AlignVCenter
            horizontalAlignment: TextInput.AlignHCenter
            color: "white"
            background: Rectangle{
                color: "#22ffffff"
                radius: 8
            }
            PlaceholderText{
                text: "请输入OICQ号"
                anchors.centerIn: parent
                font.pixelSize: 16
                color: "#55ffffff"
                visible: username.text === ""
            }
        }
        TextField {
            id: password
            width: 260
            height: 40
            text: "123456"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 8
            verticalAlignment: TextInput.AlignVCenter
            horizontalAlignment: TextInput.AlignHCenter
            echoMode: TextField.Password // 设置为密码输入模式
            color: "white"
            background: Rectangle{
                color: "#22ffffff"
                radius: 8
            }
            PlaceholderText{
                text: "请输入OICQ密码"
                anchors.centerIn: parent
                font.pixelSize: 16
                color: "#55ffffff"
                visible: password.text === ""
            }
        }
        CheckBox {
            id: radioBtn
            contentItem: Row {
                Text{
                    text: "已阅读并同意"
                    color: "#898989"
                    verticalAlignment: Text.AlignVCenter
                }
                Text{
                    text: "服务协议"
                    color: "#0066cc"
                    verticalAlignment: Text.AlignVCenter
                }
                Text{
                    text: "和"
                    color: "#898989"
                    verticalAlignment: Text.AlignVCenter
                }
                Text{
                    text: "隐私保护指引"
                    color: "#0066cc"
                    verticalAlignment: Text.AlignVCenter
                }
                opacity: parent.enabled ? 1.0 : 0.3
                leftPadding: parent.indicator.width + 5
            }

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
                border.color: radioBtn.checked ? "#0066cc" : "#888888"
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
            onCheckedChanged: {
                icon.border.color = checked ? "#0066cc" : "#888888"
            }
        }


        Button{
            id:loginBtn
            width: 260
            height: 38
            enabled: !(!radioBtn.checked || username.text == ""  || password.text == "")
            anchors.horizontalCenter: parent.horizontalCenter
            background: Rectangle{
                id:loginBtnBack
                color: enabled ? "#0066cc" : "#1f3160"
                opacity: enabled ? 1 : 0.75
                radius: 8
            }
            contentItem: Text{
                id:loginBtnText
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: "登录"
                color: parent.enabled ? "white":"#55ffffff"
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
                    Controller.connectToServer()
                    timer.restart()
                }else{
                    Controller.loginToServer(username.text, password.text, NetChat.MSG_TYPE.Login)  //登录
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
            Controller.loginToServer(username.text, password.text, NetChat.MSG_TYPE.Login)  //登录
        }
        function onLoginReturnToQml(res){
            if(res === NetChat.RETURN.Success){
                console.log("Login.qml :: 登录成功")
                loginRequest()
            }else if(res === NetChat.RETURN.Repeat){
                console.log("Login.qml :: 重复登录")
                repeatlogin.visible=true
                z=1
                repeatlogin.username=username.text

            }else if(res === NetChat.RETURN.Fail){
                console.log("Login.qml :: 账号或者密码错误")
                loginfail.visible=true
                z=1
            }
        }
    }


    Timer{
        id:timer
        interval: 2000
        onTriggered: {
            if(!connectRes){
                //messageDialog.open()
                dialogs.erroMessageDialog.open()
            }
        }
    }

    MyComponent.LoginFail{
        id:loginfail
        visible: false
        anchors.centerIn: parent
    }

    MyComponent.RepeatLogin{
        id:repeatlogin
        visible: false
        anchors.centerIn: parent
    }
    Dialogs{
        id:dialogs

    }
}
