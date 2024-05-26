import QtQuick
import QtQuick.Controls
import Qt.labs.platform
import CustomModels
import Qt5Compat.GraphicalEffects

//头像上传的弹出窗口

Popup{
    id: popup
    width: 680
    height: 560
    anchors.centerIn: parent
    modal: true
    closePolicy: Popup.CloseOnEscape  //按下退出键关闭
    property string choseHeadpath : ""
    background: Rectangle{
        color: "#1b1b1b"
        //添加背景阴影
        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            horizontalOffset: 3
            verticalOffset: 3
            radius: 5
            samples: 10
            color: "#454545"
        }
    }

    Image{
        id:head
        source: FriendModel.myImagePath
        width: 420
        height: 420
        x: (parent.width - width) / 2
        anchors.top: parent.top
        anchors.topMargin: 40
    }

    Row{
        id:row
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        x: (parent.width - width) / 2
        spacing: 35
        Button{
            id: okButton
            width: 60
            height: 30
            background: Rectangle{
                color: "#00bfff"
                radius: 5
                border.width: 1
                border.color: "#aaaaaa"
            }
            onClicked: {
                if(choseHeadpath !== ""){
                    FriendModel.updateMyHead(choseHeadpath)
                    popup.close()
                }
            }
            contentItem: Text{
                text: "上传"
                font.pixelSize: 18
                color: "#eeeeee"
                horizontalAlignment: Text.AlignHCenter
            }
        }
        Button{
            id: urlButton
            width: 100
            height: 30
            background: Rectangle{
                color: "#00bfff"
                radius: 5
                border.width: 1
                border.color: "#aaaaaa"
            }
            onClicked: {
                fileDialog.open()

            }
            contentItem: Text{
                text: "选择头像"
                font.pixelSize: 18
                color: "#eeeeee"
                horizontalAlignment: Text.AlignHCenter
            }
        }
        Button{
            id: cancelButton
            width: 60
            height: 30
            background: Rectangle{
                color: "#eeeeee"
                radius: 5
                border.width: 1
                border.color: "#aaaaaa"
            }

            onClicked: {
                head.source = FriendModel.myImagePath
                popup.close()
            }
            contentItem: Text{
                text: "取消"
                font.pixelSize: 18
                color: "#222"
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }
    FileDialog {
        id: fileDialog
        title: "请选择你的头像"
        fileMode: FileDialog.OpenFile
        folder: "./"
        nameFilters: ["Image files (*.jpg *.jpeg *.png *.bmp)", "All files (*)"]
        onAccepted: {
            choseHeadpath = fileDialog.file
            head.source = choseHeadpath
        }
    }
}
