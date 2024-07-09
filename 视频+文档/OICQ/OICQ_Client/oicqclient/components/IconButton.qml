import QtQuick
import QtQuick.Controls

//图片按钮
//按钮和图片一样大

Button {
    id: button
    implicitWidth: 50
    implicitHeight: 50
    property string imagePath: ""
    property string backColor: "#f2f2f2"
    property string hoveredImagePath: imagePath
    property string hoveredBackColor: backColor

    background: Rectangle {
        color: button.hovered ? hoveredBackColor : backColor
        radius: 5
    }

    contentItem: Image {
        source: button.hovered ? button.hoveredImagePath : button.imagePath
    }

    hoverEnabled: true
}
