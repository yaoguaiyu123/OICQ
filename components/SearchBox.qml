import QtQuick

//搜索框控件

Rectangle {
    id:box
    color: "#f2f2f2"
    implicitWidth: 245
    implicitHeight: 30
    property alias inputText: textInput.text   //开放属性
    Row{
        Rectangle{
            width: 30
            height: 30
            color: box.color
            Image{
                width: 22
                height: 22
                anchors.centerIn: parent
                source:"qrc:/icon/sousuo.png"
            }
        }
        TextInput{
            id:textInput
            width: 220
            height: 30
            verticalAlignment: Text.AlignVCenter
            clip: true
            //这里的这个Text充当placeholder的作用
            Text {
                id: placeholderText
                anchors.fill: parent
                visible: textInput.text === ""
                color: "gray"
                text: "搜索"
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

}
