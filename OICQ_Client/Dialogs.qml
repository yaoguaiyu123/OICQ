import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs as QQuickDialog
import "components" as Controller

Item {
     property alias erroMessageDialog: _erroMessageDialog
     property alias headFileDialog: _headFileDialog
     property alias uploadFileDialog: _uploadFileDialog
     property alias tipDialog: _tipDialog
     anchors.centerIn: parent

    //Connect Erro
    // MessageDialog{
    //     id:_erroMessageDialog
    //     title: qsTr("连接服务器失败，请检查你的网络")
    //     informativeText: "network error"
    //     buttons: MessageDialog.Ok

    // }
    Dialog{
        id:_erroMessageDialog
        //title: qsTr("NetWork Error")
        implicitWidth: 280
        implicitHeight: 180
        anchors.centerIn: parent
        // 自定义背景
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
        //文本内容显示
        Text{
            id:tipsText
            Layout.topMargin: 25
            Layout.alignment: Qt.AlignCenter
            font.pixelSize: 16
            font.family: "微软雅黑"
            textFormat: Text.Html
            text: "连接服务器失败，请检查你的网络\n"+
                  "        NetWork Error\n"

        }
        //自定义确认按钮
          Button {
             id:ok
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
    QQuickDialog.FileDialog {
        id: _headFileDialog
        title: "选择图片"
        fileMode: QQuickDialog.FileDialog.OpenFile
        nameFilters: ["图片文件 (*.png *.jpg *.jpeg *.bmp)"]
        acceptLabel: "打开"
        rejectLabel: "取消"
        onAccepted: {
            mainView.headPopup.choseHeadpath = _headFileDialog.selectedFile
            mainView.headPopup.head.source = _headFileDialog.selectedFile
            // console.log("ddd ",choseHeadpath)
        }
    }

    //上传文件
    FileDialog {
        id: _uploadFileDialog
        title: "选择文件"
        fileMode: FileDialog.OpenFiles
        acceptLabel: "Open"
        rejectLabel: "Cancel"
        //MessageInputField=>MessageList=>rightView=>frientChatWindow=>mainView
        onAccepted: {
            // console.log("用户选择的文件: " + fileDialog.selectedFile)
            //sendFile(_uploadFileDialog.selectedFiles)
            mainView.windowLoader.item.messagelist.inputField.sendFile(_uploadFileDialog.selectedFiles)
        }
    }

    //提示选择文件上限
    // MessageDialog{
    //     id: _tipDialog
    //     title: qsTr("一次最多只能上传5个文件")
    //     informativeText: "请重新选择"
    //     buttons: MessageDialog.Ok
    // }
    Dialog{
        id:_tipDialog
        //title: qsTr("提示")
        implicitWidth: 280
        implicitHeight: 180
        anchors.centerIn: parent
        // 自定义背景
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
        //文本内容显示
        Text{
            id:_tipsText
            Layout.topMargin: 25
            Layout.alignment: Qt.AlignCenter
            font.pixelSize: 16
            font.family: "微软雅黑"
            text: "一次最多只能上传5个文件\n"+
                  "        请重新选择!\n"
        }
        //自定义确认按钮
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

