import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts

//登录失败页面
Popup{
    id:content
    implicitWidth: 280
    implicitHeight: 180
    modal: true
    background: Rectangle{
        anchors.fill: parent
        color: "white"
        radius: 5
    }

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
            Layout.alignment: Qt.AlignRight
            font.pixelSize: 16
            font.family: "微软雅黑"
            text: "登录失败\n\n"+
                  "账号或密码输入错误 ，"+"请重新登录。\n"
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
