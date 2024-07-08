import QtQuick
import QtQuick.Dialogs as QuickDialogs
import QtQuick.Controls
import QtQuick.Layouts
import "components" as Controller

Item {
     property alias erroMessageDialog: _erroMessageDialog
     property alias headFileDialog: _headFileDialog
     property alias uploadFileDialog: _uploadFileDialog
     property alias tipDialog: _tipDialog
     anchors.centerIn: parent

    Dialog{
        id:_erroMessageDialog
        implicitWidth: 280
        implicitHeight: 180
        anchors.centerIn: parent
        background: Rectangle {
            color: "white"
            radius: 5
            anchors.fill: parent
        }

        ColumnLayout{

            Controller.BackIconButton{
               id:iconButton
               Layout.alignment: Qt.AlignRight
               imagePath: "qrc:/icon/close.png"
               imageHoverPath: "qrc:/icon/close_w.png"
               backColor: "#00000000"
               backHoverColor: "#cc4444"
               onClicked: {
                  _erroMessageDialog.close()
               }

            }
            Text{
                Layout.topMargin: 15
                font.pixelSize: 16
                font.family: "微软雅黑"
                text: " 连接服务器失败，请检查你的网络\n"

            }
            Text{
                font.pixelSize: 16
                text: "   network error\n"
            }

            Button {
               id:ok
               Layout.preferredWidth: 60
               Layout.preferredHeight: 24
               Layout.leftMargin: 200
              // Layout.topMargin: 10
               contentItem: Text{
                  text: "确认"
                  color:"white"
                  font.pixelSize: 16
                  font.family: "微软雅黑"
                  verticalAlignment: Text.AlignVCenter
                  horizontalAlignment: Text.AlignHCenter
               }
               background: Rectangle{
                  anchors.fill: parent
                  color: ok.hovered ? "#1f3160" : "#0066cc"
                  radius: 4
               }

               onClicked:{
                  _erroMessageDialog.close()
               }
            }

        }
    }

    //头像上传弹窗
    QuickDialogs.FileDialog {
        id: _headFileDialog
        title: "选择图片"
        fileMode: QuickDialogs.FileDialog.OpenFile
        nameFilters: ["图片文件 (*.png *.jpg *.jpeg *.bmp)"]
        acceptLabel: "打开"
        rejectLabel: "取消"
        onAccepted: {
            mainView.headPopup.choseHeadpath = _headFileDialog.selectedFile
            mainView.headPopup.head.source = _headFileDialog.selectedFile
        }
    }

    //上传文件
    QuickDialogs.FileDialog {
        id: _uploadFileDialog
        title: "选择文件"
        fileMode: QuickDialogs.FileDialog.OpenFiles
        acceptLabel: "Open"
        rejectLabel: "Cancel"
        //MessageInputField=>MessageList=>rightView=>frientChatWindow=>mainView
        onAccepted: {
            mainView.windowLoader.item.messagelist.inputField.sendFile(_uploadFileDialog.selectedFiles)
        }
    }

    //提示选择文件上限
    Dialog{
        id:_tipDialog
        implicitWidth: 280
        implicitHeight: 180
        anchors.centerIn: parent
        background: Rectangle {
            color: "white"
            radius: 5
            anchors.fill: parent
        }

        ColumnLayout{
            Controller.BackIconButton{
                id:_iconButton
                Layout.alignment: Qt.AlignRight
                imagePath: "qrc:/icon/close.png"
                imageHoverPath: "qrc:/icon/close_w.png"
                backColor: "#00000000"
                backHoverColor: "#cc4444"
                onClicked: {
                   _tipDialog.close()
                }
            }

            Text{
                id:_tipsText
                Layout.topMargin: 25
                Layout.alignment: Qt.AlignCenter
                font.pixelSize: 16
                font.family: "微软雅黑"
                text: "一次最多只能上传5个文件\n"+
                      "        请重新选择!\n"
            }
              Button {
                 id:_ok
                 implicitWidth: 60
                 implicitHeight: 24
                 Layout.leftMargin: 200
                 Layout.bottomMargin: 20
                 contentItem: Text{
                     text: "确认"
                     color:"white"
                     font.pixelSize: 16
                     font.family: "微软雅黑"
                     verticalAlignment: Text.AlignVCenter
                     horizontalAlignment: Text.AlignHCenter
                 }
                 background: Rectangle{
                     anchors.fill: parent
                     color: _ok.hovered ? "#1f3160" : "#0066cc"
                     radius: 4
                 }

                 onClicked:{
                     _tipDialog.close()
                 }
              }
        }
    }
}

