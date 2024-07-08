import QtQuick
import QtQuick.Controls
import "../components" as MyComponent
import oicqclient
import QtQuick.Layouts
import "../comman/NetChat.js" as NetChat
import Qt5Compat.GraphicalEffects

//添加好友的弹窗
Popup {
    id:popup
    implicitWidth: 220
    implicitHeight: 100
    ColumnLayout{
        anchors.fill: parent
        RowLayout{
            MyComponent.SearchBox{
                id:searchbox
                Layout.preferredWidth: 140
                onInputTextChanged: {
                    showText.text = ""
                    showIcon.source = ""
                }
            }
            Button{
                Layout.preferredWidth: 60
                Layout.preferredHeight: 28
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
                    Controller.sendAddFriendRequest(searchbox.inputText ,NetChat.MSG_TYPE.AddFriend)
                }
            }
        }
        Rectangle{   //用来回显的控件
            Layout.preferredWidth: 190
            Layout.preferredHeight: 40
            Layout.leftMargin: popup.width / 2 - width / 2
            RowLayout{
                spacing: 5
                anchors.centerIn: parent
                Image{
                    id:showIcon
                    Layout.preferredWidth: 20
                    Layout.preferredHeight: 20
                    source: ""
                }
                Text{
                    id:showText
                    font.pixelSize: 16
                    wrapMode: Text.WrapAnywhere
                    Layout.preferredHeight: showIcon.height
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

    }


    Connections{
        target: TcpSocket
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
