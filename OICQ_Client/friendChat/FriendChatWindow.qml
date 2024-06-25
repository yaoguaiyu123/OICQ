import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import oicqclient

//该窗口为好友聊天窗口
Item {
     id:friendChatWindow
     anchors.fill: parent
     property alias rightview: rightView
     property alias messagelist: messageList
     property var heads
     property int currentIndex: 0

    SplitView{
        id: split1
        anchors.fill: parent
        orientation: Qt.Horizontal
        handle: splitHandle

        SplitView{
            id: leftView
            SplitView.preferredWidth: 310
            SplitView.maximumWidth: 310
            SplitView.minimumWidth: 180
            RecentFriendList{
                id:recentFriendList
                SplitView.fillHeight: true
                implicitWidth: 310
                onCurrentIdChanged: {
                    currentIndex = currentId
                    messageList.updateModel(currentIndex)
                }
            }

        }

        SplitView {
            id: rightView
            SplitView.fillWidth: true
            SplitView.minimumWidth: 400
            ColumnLayout{

                MessageList{
                    id:messageList
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
            }

        }

        Component {
                id: splitHandle

                Rectangle {
                    implicitWidth: 4
                    implicitHeight: 4
                    opacity: 0
                }
            }
    }
}
