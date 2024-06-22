import QtQuick
import QtQuick.Controls
import "../components" as MyComponent
import oicqclient
import QtQuick.Layouts
import "../comman/NetChat.js" as NetChat
import Qt5Compat.GraphicalEffects

Popup {
    id:popup
    width: 220
    height: 100
    Row{
        MyComponent.SearchBox{
            id:searchbox
            implicitWidth: 145
            onInputTextChanged: {
                showText.text = ""
                showIcon.source = ""
            }
        }
        Button{
            width: 60
            height: 30
            contentItem: Text {
                text: qsTr("添加好友")
                color: "white"
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle{
                color: "#0099ff"
            }
            onClicked: {
                //添加好友
                Controller.packingMessage("{" + "\"friendId\":" + searchbox.inputText + "}",NetChat.MSG_TYPE.AddFriend)
            }
        }
    }
    Rectangle{
        //用来回显的控件
        width: 190
        x:parent.width / 2 - width / 2
        y: 40
        height: 40
        Row{
            spacing: 5
            anchors.centerIn: parent
            Image{
                id:showIcon
                width: 20
                height: 20
                source: ""
            }
            Text{
                id:showText
                font.pixelSize: 16
                wrapMode: Text.WrapAnywhere
                height: showIcon.height
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    Connections{
        target: Controller.getTcpSocket()
        function onAddFriend(res){
            if(res === NetChat.RETURN.Fail){
                showText.text = "没有找到该用户"
                showText.color = "red"
                showIcon.source = "qrc:/icon/fail.png"
            }else if(res === NetChat.RETURN.Success){
                showText.text = "成功发送请求"
                showText.color = "black"
                showIcon.source = "qrc:/icon/success.png"
            }else if(res === NetChat.RETURN.Repeat){
                showText.text = "该用户已经是你的好友"
                showText.color = "red"
                showIcon.source = "qrc:/icon/fail.png"
            } else if(res === NetChat.RETURN.Repeat2){
                showText.text = "重复发送好友申请"
                showText.color = "red"
                showIcon.source = "qrc:/icon/fail.png"
            }
        }
    }

    background: Rectangle{
        color: "white"
        //添加背景阴影
        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            horizontalOffset: 1
            verticalOffset: 1
            radius: 8
            samples: 10
            color: "#454545"
        }
    }
}
