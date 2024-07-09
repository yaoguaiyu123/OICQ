import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components" as MyComponent
import oicqclient

//好友聊天窗口的左边部分
Rectangle {
    id:container
    color: "white"
    property var heads
    property int currentId: 0
    signal currentClicked()

    FriendsGroupMenu{
        id:friendsGroupMenu
        //位于layout范围之外的位置确定
        x: addButton.x + addButton.width
        y: addButton.y + addButton.height / 2
        onOpenFgPop: {
            fgPopup.open()
        }
    }

    FriendsGroupPopup{
        id:fgPopup
        x: friendChatWindow.width / 2 - width / 2
        y: friendChatWindow.height / 2 - height / 2
    }

    ColumnLayout{
        RowLayout{
            id:topRow
            spacing: 10
            MyComponent.SearchBox{
                Layout.preferredWidth: 250- (310-container.width)
                Layout.leftMargin: 12
                Layout.topMargin: 30
            }
            MyComponent.IconButton{
                id:addButton
                imagePath: "qrc:/icon/jia.png"
                backColor: "#f2f2f2"
                hoveredBackColor: "#dcdcdc"
                Layout.preferredHeight: 30
                Layout.preferredWidth: 30
                Layout.topMargin: 30
                onClicked: {
                    friendsGroupMenu.open()
                }
            }
        }

        ListView{
            Layout.topMargin: 10
            Layout.preferredWidth: container.width
            Layout.preferredHeight: container.height - topRow.height - 30
            clip: true
            model: FriendModel
            delegate:RecentFriendListDelegate{
                backColor: currentId == index ? "#0099ff" : "white"
                bigTextColor: currentId == index ? "white" : "black"
                smallTextColor: currentId == index ? "white" : "#999999"
                onClicked: {
                    currentId = index
                    if(FriendModel.rowCount() === 1){
                        currentClicked()
                    }
                }
            }
        }
    }



}
