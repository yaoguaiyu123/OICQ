import QtQuick
import QtQuick.Layouts
import "../components" as MyComponent
import QtQuick.Controls
import CustomModels

// 消息样式

Item {
    id:container
    //NOTE
    //这里ListView.view.width获取失败，不知道为啥
    width: viewWidth
    height: layout.visible ? layout.height : tiptext.height
    property string headUrl
    property int viewWidth: 120
    Text{
        id:tiptext
        width: viewWidth
        font.pixelSize: 12
        color: "#aaaaaa"
        visible: type === "sendfile" || type === "recvfile" || type === "send"
                 || type === "recv" ? false : true
        text: type === "tip" ? msg : new Date().toLocaleString(Qt.locale(), "yyyy/MM/dd hh:mm:ss")
        horizontalAlignment: Text.AlignHCenter
    }

    RowLayout{
        id: layout
        visible: type === "sendfile" || type === "recvfile" || type === "send"
                 || type === "recv" ? true : false
        layoutDirection: type === "send" || type === "sendfile" ? Qt.RightToLeft : Qt.LeftToRight
        spacing: 10
        width: type === "send" || type === "recv" ?
                   textContainer.implicitWidth + headImage.implicitWidth :
                   filePattern.implicitWidth + headImage.implicitWidth
        height: type === "send" || type === "recv" ?
                    Math.max(textContainer.implicitHeight, headImage.implicitWidth) :
                    Math.max(filePattern.implicitHeight , headImage.implicitHeight)
        MyComponent.HeadImage {
            id:headImage
            width: 36
            height: 36
            imageRadius: 18
            headUrl: type === "send" || type === "sendfile" ? FriendModel.myImagePath : container.headUrl
        }
        ChatBubble{
            id:textContainer
            visible: type === "send" || type === "recv" ? true : false
        }
        ChatFile{
            id:filePattern
            height: 80
            width: 220
            visible: type === "sendfile" || type === "recvfile" ? true : false
        }

        Component.onCompleted: {
            if(type === "send" || type === "sendfile"){
                anchors.right = container.right
            }
        }

    }

}
