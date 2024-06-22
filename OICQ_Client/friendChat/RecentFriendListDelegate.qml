import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components" as MyComponent
import oicqclient

Button {
    id:button
    width: ListView.view.width
    height: 70
    property string backColor: "white"
    property string bigTextColor : "black"
    property string smallTextColor : "#999999"
    background: Rectangle{
        color: button.backColor
    }
    contentItem: Rectangle{
        id:content
        color: button.backColor
        //横向定位
        RowLayout{
            width: content.width
            implicitHeight: 70
            id:row
            anchors.verticalCenter: parent.verticalCenter
            spacing: 9
            MyComponent.HeadImage{
                id:headimage
                Layout.preferredHeight: 40
                Layout.preferredWidth: 40
                imageRadius: 20
                headUrl: headPath
            }
            ColumnLayout{
                spacing: 9
                Layout.preferredHeight: 40
                Layout.fillWidth: true
                Text{
                    id: username
                    font.pixelSize: 14
                    text: name
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignLeft
                    //TODO 先简单实现逻辑
                    color:name === "小爱同学" && bigTextColor === "black" ? "#10a37f" : button.bigTextColor
                }
                Text {
                    id: lastMsg
                    font.pixelSize: 12
                    color: button.smallTextColor
                    text: recentMessage
                    width: button.width - headimage.width
                    height: 16
                    elide: Text.ElideRight
                    horizontalAlignment: Text.AlignLeft
                    Layout.fillWidth: true

                }

            }
        }
        //时间不包括在Row定位中，而是根据Row进行定位
        Text{
            id:dateText
            anchors.right: row.right
            anchors.top: row.top
            text: getRecentDate()
            color: button.smallTextColor
            font.pixelSize: 12
        }

        // 当前未读消息数量
        Rectangle{
            width: 24
            height: 16
            anchors.right: row.right
            anchors.bottom: row.bottom
            radius: 8
            visible: unreadMessageNum > 0
            color: "#ff2222"
            Text{
                anchors.centerIn: parent
                color: "white"
                text: unreadMessageNum
                font.pixelSize: 11
            }
        }
    }

    function getRecentDate () {
        var currentDate = new Date()
        // var date = ListModel.
        // TODO
        var date = new Date()
        var total = currentDate-date

        if (total < (1000*60*60*24)) {
            return date.toLocaleTimeString(Qt.locale(), "hh:mm:ss")
        } else if (total < (1000*60*60*24) * 2) {
            return "昨天";
        } else if (total < (1000*60*60*24) * 3) {
            return "前天";
        } else {
            return date.toLocaleDateString(Qt.locale(), "yyyy/MM/dd")
        }
    }
}
