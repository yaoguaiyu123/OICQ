import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts

//重复登录弹窗
Popup{
    id:content
    property string username: "123456"//初始化登录用户的名字
    background: Rectangle{
        anchors.fill: parent
        color: "white"
        radius: 5
    }
    implicitWidth: 280
    implicitHeight: 180
    modal: true

    ColumnLayout{
        BackIconButton{
           id:iconButton
           Layout.alignment: Qt.AlignRight
           imagePath: "qrc:/icon/close.png"
           imageHoverPath: "qrc:/icon/close_w.png"
           backColor: "#00000000"
           backHoverColor: "#cc4444"
           onClicked: {
              content.close()
           }

       }

       Text{
            id:tipsText
            font.pixelSize: 16
            font.family: "微软雅黑"
            text: " 提示\n\n"+
                  " 你已登录了一个相同的账号 "+username+"，\n 请勿重复登录！"
             verticalAlignment: TextInput.AlignVCenter
        }

       Button{
           id: _ok
           implicitWidth: 60
           implicitHeight: 24
           Layout.leftMargin: 200
           Layout.bottomMargin: 20
           contentItem: Text{
               text: "确认"
               color:"white"
               font.pixelSize: 16
               font.family: "微软雅黑"
               verticalAlignment: Text.AlignVCenter
               horizontalAlignment: Text.AlignHCenter
           }
           background: Rectangle{
               anchors.fill: parent
               color: _ok.hovered ? "#1f3160" : "#0066cc"
               radius: 4
           }

           onClicked: {
               content.close()
           }

       }
    }

}
