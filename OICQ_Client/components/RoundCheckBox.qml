import QtQuick 2.0
import QtQuick.Controls



Menu{
    //判断是否隐藏窗口按钮被触发
    property bool checked: false
    //窗口按钮触发信号
    signal isChecked
    signal noChecked

    id:tipsMenu
    Action{
        text: qsTr("")
        onTriggered: {
            checked=!checked
            if(checked===true){
                //todo
                isChecked()
            }else{
                noChecked()
            }
        }
    }

    delegate: MenuItem{
        id:menuItem
        implicitWidth: 180
        implicitHeight: 27
        leftInset: 10
        topInset: 5
        font.pixelSize: 15

        // contentItem: Text {
        //             text: menuItem.text
        //             font: menuItem.font
        //             opacity: enabled ? 1.0 : 0.3
        //             horizontalAlignment: Text.AlignHCenter
        //             verticalAlignment: Text.AlignVCenter
        //             elide: Text.ElideRight
        //         }

        contentItem:
            //一个圆形按钮组件作为内容
            Rectangle{
            implicitHeight: 27
            implicitWidth: 100
            anchors.centerIn: parent

           Row{

           //圆形按钮
           Rectangle{
                id:roundBotton
                width: 15
                height: width
                radius: width/2
                color: checked?"#0099ff" : "#f2f2f2"
                anchors.leftMargin: 5
                //anchors.right:hintText.left
                anchors.rightMargin: 15
           }

           //提示文本
           Rectangle{
               color:"transparent"
           Text {
               id:hintText
               text: "     隐藏当前窗口截图"
               font.pixelSize:roundBotton.height
               color :"#333333"

           }

           }
           }
        }

        //菜单项背景
        background: Rectangle{
            anchors.centerIn: parent
            implicitHeight: 30
            implicitWidth: 100
            opacity: enabled ? 1 : 0.3
            color:menuItem.highlighted?"#e6e6e6" : "white"
            radius: 5
        }
    }

    //菜单背景
    background: Rectangle{
        implicitHeight: 30
        implicitWidth: 180
        color: "white"
        radius: 3
        anchors.fill: parent
    }

}
