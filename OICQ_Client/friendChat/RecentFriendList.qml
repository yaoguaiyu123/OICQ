import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components" as MyComponent
import oicqclient

Rectangle {
    id:container
    color: "white"
    property var heads

    property int currentId: 0

    FriendsGroupMenu{
        id:friendsGroupMenu
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

    //行布局
    Row{
        id:topRow
        x:15
        y: 30
        spacing: 10
        MyComponent.SearchBox{
            width: 250- (310-container.width)
        }
        MyComponent.IconButton{
            id:addButton
            imagePath: "qrc:/icon/jia.png"
            backColor: "#f2f2f2"
            hoveredBackColor: "#dcdcdc"
            height: 30
            width: 30
            onClicked: {
                friendsGroupMenu.open()
            }
        }
        z:2
    }
    ListView{
        y: topRow.y + topRow.height + 10
        width: container.width
        height: container.height - topRow.height - 30
        clip: true
        model: FriendModel
        delegate:RecentFriendListDelegate{
            backColor: currentId == index ? "#0099ff" : "white"
            bigTextColor: currentId == index ? "white" : "black"
            smallTextColor: currentId == index ? "white" : "#999999"
            onClicked: {
                currentId = index  //更改颜色(蓝色 or 白色)
            }
        }
    }
}
