import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts

//截图时选择是否隐藏当前窗口的小弹窗
Menu{
    id:tipsMenu
    property bool checked: false
    signal isChecked
    signal noChecked

    background: Rectangle{
        implicitHeight: 30
        implicitWidth: 180
        color: "#fff"
        radius: 5
        anchors.fill: parent
    }
    Action{
        text: qsTr("")
        onTriggered: {
            checked =! checked
            if(checked === true){
                isChecked()
            }else{
                noChecked()
            }
        }
    }

    delegate: MenuItem{
        id:menuItem
        font.pixelSize: 15
        background: Rectangle{
            anchors.centerIn: parent
            implicitHeight: 26
            implicitWidth: 168
            color: menuItem.hovered ? "#e6e6e6" : "white"
        }

        contentItem: Rectangle{
            anchors.centerIn: parent
            color: menuItem.hovered ? "#e6e6e6" : "white"
            radius: 4
            RowLayout{
               Rectangle{    //圆形按钮
                    id:roundBotton
                    Layout.preferredWidth: 14
                    Layout.preferredHeight: 14
                    radius: 7
                    color: checked ? "#0099ff" :"#ccc"
                    Layout.leftMargin: 5
               }

               Text {
                   id:hintText
                   text: "  隐藏当前窗口截图"
                   font.pixelSize: roundBotton.height
                   color :"#333333"
               }
           }
        }

    }

}
