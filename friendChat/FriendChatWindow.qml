import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../comman/Comman.js" as Comman
import CustomModels
//该窗口为好友聊天窗口

Item{
    id:friendChatWindow
    Layout.fillHeight: true
    Layout.fillWidth: true

    property var heads
    property int currentIndex: 0

    //行布局
    RowLayout{
        id:window1_rowlayout
        anchors.fill: parent
        //最近好友列表
        RecentFriendList{
            id:recentFriendList
            Layout.fillHeight: true
            implicitWidth: 310
            onCurrentIdChanged: {
                currentIndex = currentId
                messageList.updateModel(currentIndex)
            }
        }
        //聊天窗口
        MessageList{
            id:messageList
            Layout.fillHeight: true
            Layout.fillWidth: true
            Component.onCompleted: {
                messageList.dataModel = FriendModel
            }
        }
    }
}
