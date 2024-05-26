import QtQuick 2.0
import QtQuick.Controls 2.5

//可移动窗口

MouseArea {
    id: container
    property var control
    property point movePoint
    // 接收鼠标按下事件
    onPressed: {
        movePoint = Qt.point(mouseX,mouseY)
        // console.log("press",mouseX,mouseY)
    }

    onPositionChanged: {
        if (pressed ) {
            control.x += mouseX - movePoint.x;
            control.y += mouseY - movePoint.y;
        }
    }

}
