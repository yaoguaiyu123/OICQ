import QtQuick
import QtQuick.Layouts
import "../components" as MyComponent
import QtQuick.Controls
import CustomModels

// 消息样式

Item {
    id:container
    implicitWidth: viewWidth
    implicitHeight: type === "tip" || type === "tipDate" ? 20 : messageRow.implicitHeight

    property int viewWidth: 0
    property string headUrl
    property string myheadUrl
    signal uploadFile1(var filepath, var index)
    signal updateListView()

    Text{
        id:tiptext
        width: parent.width
        font.pixelSize: 12
        color: "#aaaaaa"
        visible: type === "sendfile" || type === "recvfile" || type === "send"
                 || type === "recv" ? false : true
        text: type === "tip" ? msg : new Date().toLocaleString(Qt.locale(), "yyyy/MM/dd hh:mm:ss")
        horizontalAlignment: Text.AlignHCenter
    }

    Row{
        id: messageRow
        spacing: 10
        visible: type === "sendfile" || type === "recvfile" || type === "send"
                 || type === "recv" ? true : false
        anchors.right: type === "send" || type === "sendfile" ? parent.right : undefined
        layoutDirection: type === "send" || type === "sendfile" ? Qt.RightToLeft : Qt.LeftToRight


        MyComponent.HeadImage {
            id:avatar
            width: 36
            height: 36
            imageRadius: 18
            headUrl: type === "recv" || type === "recvfile" ?  container.headUrl : container.myheadUrl
        }

        Rectangle {
            id: bubble
            width: Math.min(
                       messageText.implicitWidth + 24,
                       0.6 * (viewWidth - avatar.width - messageRow.spacing))
            height: messageText.implicitHeight + 24
            color: type === "send" ? "#0099ff" : "white"
            radius: 10
            visible: type === "send" || type === "recv" ? true : false
            TextEdit {
                //消息
                id: messageText
                text: msg === undefined ? "" : msg
                color: type === "send" ? "white" : "black"
                anchors.fill: parent
                anchors.margins: 12
                wrapMode: TextEdit.WrapAnywhere
                font.pixelSize: 14
                readOnly: true      //设置为只读
                textFormat: TextArea.MarkdownText

            }
        }

        ChatFile{
            id:filePattern
            width: 180
            height: 60
            visible: type === "sendfile" || type === "recvfile" ? true : false
            fileName: filename === undefined ? "" : filename
            fileSize: filesize === undefined ? "" : filesize
            onUploadFile:(filepath)=> {
                uploadFile1(filepath,index)
            }
        }

    }

}
