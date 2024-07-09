import QtQuick
import QtQuick.Controls

//图片按钮
//按钮比图片大

Button {
    id: customButton
    implicitWidth: 40
    implicitHeight: 30
    leftPadding: 10
    rightPadding: 10
    property string backColor: "#f2f2f2"
    property string backHoverColor: "#dfdfdf"
    property string imagePath: ""
    property string imageHoverPath: imagePath

    background: Rectangle {
        color: customButton.hovered ? backHoverColor : backColor
    }

    contentItem: Image {
        width: 15
        height: 15
        source: customButton.hovered ? customButton.imageHoverPath : customButton.imagePath
        anchors.centerIn: parent
    }

    hoverEnabled: true
}
