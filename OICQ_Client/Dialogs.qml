import QtQuick
import QtQuick.Dialogs
import QtQuick.Dialogs as QQuickDialog

Item {
     property alias erroMessageDialog: _erroMessageDialog
     property alias headFileDialog: _headFileDialog
     property alias uploadFileDialog: _uploadFileDialog
     property alias tipDialog: _tipDialog


    //Connect Erro
    MessageDialog{
        id:_erroMessageDialog
        title: qsTr("连接服务器失败，请检查你的网络")
        informativeText: "network error"
        buttons: MessageDialog.Ok
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
    MessageDialog{
        id: _tipDialog
        title: qsTr("一次最多只能上传5个文件")
        informativeText: "请重新选择"
        buttons: MessageDialog.Ok
    }
}

