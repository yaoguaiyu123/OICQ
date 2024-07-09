import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

//点击加号弹出的菜单，添加好友和发起群聊
Menu{
    implicitWidth: 120
    implicitHeight: 80
    id:menu
    signal openFgPop()
    delegate: MenuItem{
        id:menuItem
        height: 40
        width: 120
        contentItem: Text {
            text: menuItem.text
            font: menuItem.font
            opacity: enabled ? 1.0 : 0.3
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 30
            radius: 3
            anchors.centerIn: parent
            opacity: enabled ? 1 : 0.3
            color: menuItem.highlighted ? "#e6e6e6" : "white"
        }
    }
    Action{
        text: qsTr("添加好友")
        onTriggered: {
            openFgPop()
        }
    }
    Action{
        text: qsTr("发起群聊")
        onTriggered: {
        }
    }

    background: Rectangle{
        color: "white"
        radius: 3
        anchors.fill: parent
        //添加背景阴影
        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            horizontalOffset: 1
            verticalOffset: 1
            radius: 5
            samples: 10
            color: "#454545"
        }
    }
}



