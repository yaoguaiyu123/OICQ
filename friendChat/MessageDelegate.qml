import QtQuick
import QtQuick.Layouts
import "../components" as MyComponent
import QtQuick.Controls
import CustomModels

Item {
    id:container
    //NOTE
    //这里ListView.view.width获取失败，不知道为啥
    width: viewWidth
    height: layout.visible ? textContainer.height : tiptext.height
    property string headUrl
    property int viewWidth: 120
    Text{
        id:tiptext
        width: viewWidth
        font.pixelSize: 12
        color: "#aaaaaa"
        visible: type === "send" || type === "recv" ? false : true
        text: type === "tip" ? msg : new Date().toLocaleString(Qt.locale(), "yyyy/MM/dd hh:mm:ss")
        horizontalAlignment: Text.AlignHCenter
    }

    //这里使用Row布局，而不是RowLayout，因为其子元素大小需要动态变化
    Row{
        id: layout
        visible: type === "send" || type === "recv" ? true : false
        layoutDirection: type === "send" ? Qt.RightToLeft : Qt.LeftToRight
        spacing: 10
        MyComponent.HeadImage {
            id:headImage
            width: 36
            height: 36
            imageRadius: 18
            headUrl: type === "send"? FriendModel.myImagePath : container.headUrl
        }
        ChatBubble{
            id:textContainer
        }


        Component.onCompleted: {
            if(type === "send"){
                anchors.right = container.right
            }
        }

    }

}
