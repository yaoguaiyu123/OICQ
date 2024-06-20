import QtQuick 2.0
import QtQuick.Controls


//重复登陆弹窗
Rectangle{

    property string username: "123456"//初始化登录用户的名字

    id:content
    color: "white"
    radius: 10
    width: parent.width*0.8
    height: parent.width*0.5

    BackIconButton{
        id:iconButton
        // anchors.right: parent.right
        // anchors.top: parent.top
        imagePath: "qrc:/icon/close.png"
        imageHoverPath: "qrc:/icon/close_w.png"
        backColor: "#00000000"
        backHoverColor: "#cc4444"
        onClicked: {
            content.visible=false
            z=-1//关闭应用
        }
        anchors.right: tipsText.right
        anchors.rightMargin: 3
        anchors.top:parent.top
        anchors.topMargin: 3
        // anchors.bottom: tipsText.top
        // anchors.bottomMargin: 20
    }

    Text{
        id:tipsText
        // width: 200
        // height: 50
        //anchors.centerIn: parent
        anchors.top: iconButton.top
        anchors.topMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 18
        font.family: "微软雅黑"
        text: "提示\n\n"+
              "你已登录了一个相同的账号 "+username+" ， 请勿重复登录！"
         verticalAlignment: TextInput.AlignVCenter
        // horizontalAlignment: TextInput.AlignHCenter
    }
    Button{
        anchors.right: tipsText.right
        anchors.rightMargin: 30
        anchors.top: tipsText.bottom
        anchors.topMargin: 30
        //anchors.bottom: parent.bottom
        //anchors.bottomMargin: 30
        contentItem: Text{
            text: "确认"
            color:"white"
            font.pixelSize: 20
            font.family: "微软雅黑"
            verticalAlignment: Text.AlignVCenter
            anchors.centerIn: back
        }
        background: Rectangle{
            id:back
            width: 80
            height: 38
            color: "#0066cc"
            radius: 8
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onEntered: parent.color = "#1f3160"
                onExited: parent.color = "#0066cc"
            }
        }
        onClicked:{
            content.visible=false
            z=-1//关闭应用
        }

    }
}
