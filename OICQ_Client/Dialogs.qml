import QtQuick
import QtQuick.Dialogs as QQuickDialog

Item {
    // property alias erroMessageDialog: _erroMessageDialog
    // property alias headFileDialog: _headFileDialog

    // //Connect Erro
    // MessageDialog{
    //     id:_erroMessageDialog
    //     title: qsTr("连接服务器失败，请检查你的网络")
    //     informativeText: "network error"
    //     buttons: MessageDialog.Ok
    // }
    // QQuickDialog.FileDialog {
    //     id: _headFileDialog
    //     title: "选择图片"
    //     fileMode: QQuickDialog.FileDialog.OpenFile
    //     nameFilters: ["图片文件 (*.png *.jpg *.jpeg *.bmp)"]
    //     acceptLabel: "打开"
    //     rejectLabel: "取消"
    //     // onAccepted: {
    //     //     choseHeadpath = fileDialog.selectedFile
    //     //     head.source = fileDialog.selectedFile
    //     //     // console.log("ddd ",choseHeadpath)
    //     // }
    // }
}
