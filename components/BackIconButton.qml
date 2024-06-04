import QtQuick

//图片按钮
//按钮比图片大

Rectangle {
    id:button
    implicitWidth: 40
    implicitHeight: 30
    property string backColor: "#f2f2f2"
    property string backHoverColor: "#dfdfdf"
    property string imagePath: ""
    property string imageHoverPath: imagePath
    property alias userButton: mouseArea   //开放子组件
    property int imageWidth: width - 4
    property int imageHeight: width - 4
    color: mouseArea.containsMouse ? backHoverColor : backColor
    Image{
        id:image
        height:imageHeight
        width:imageWidth
        anchors.centerIn: parent
        source: mouseArea.containsMouse ? imageHoverPath : imagePath
    }
    MouseArea{
        id:mouseArea
        anchors.fill: parent
        hoverEnabled: true
    }
}
