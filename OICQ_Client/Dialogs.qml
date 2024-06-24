import QtQuick
import QtQuick.Dialogs as QQuickDialog

Item {
     property alias erroMessageDialog: _erroMessageDialog
     property alias headFileDialog: _headFileDialog

    //Connect Erro
    QQuickDialog.MessageDialog{
        id:_erroMessageDialog
        title: qsTr("连接服务器失败，请检查你的网络")
        informativeText: "network error"
        buttons: QQuickDialog.MessageDialog.Ok
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
}
