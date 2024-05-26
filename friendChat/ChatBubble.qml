import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle{
    id:container
    width: chatText.width + 16
    height: chatText.height + 12
    color: type === "send" ? "#0099ff" : "white"
    radius: 8
    TextArea{
        id:chatText
        anchors.centerIn: parent
        color: type === "send" ? "white" : "black"
        textFormat: TextArea.MarkdownText
        text:  msg
        wrapMode: TextArea.WrapAnywhere  //自动换行
        // lineHeight: 1.3
        font.pixelSize: 14
        readOnly: true      //设置为只读
    }
    Component.onCompleted: {
        if(chatText.width > 360){
            chatText.width = 360
        }
    }
}
