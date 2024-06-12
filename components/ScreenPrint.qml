import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Window 2.14

// 截屏窗口

Window {
    id:screenShotWindow
    property int borderMargin: 3
    property color tranparentColor: "transparent" //透明色
    property color blurryColor: Qt.rgba(0,0,0,0.3) //半透明
    property color selectBorderColor: "#0099ff" //边框色
    property rect captureArea: Qt.rect(0, 0, 0, 0) // 记录最终选择的范围

    signal provideArea(var area)

    color: blurryColor
    flags: Qt.FramelessWindowHint // 取消默认标题栏

    // 全局的mouseArea
    MouseArea {
        id: mainMouseArea
        anchors.fill: parent
        property int startX: 0
        property int startY: 0
        property int endX: 0
        property int endY: 0

        onPressed:(mouse)=>{
            startX = mouse.x
            startY = mouse.y
            selectionRect.width = 0
            selectionRect.height = 0
            selectionRect.visible = true   //显示选择区域
            captureArea = Qt.rect(startX,startY, 0 , 0)
        }

        onPositionChanged:(mouse)=>{
            if (pressed) {
                endX = mouse.x
                endY = mouse.y
                selectionRect.width = Math.abs(endX - startX)
                selectionRect.height = Math.abs(endY - startY)
                selectionRect.x = Math.min(startX, endX)
                selectionRect.y = Math.min(startY, endY)
            }
        }
        onReleased:{
            updateStartAndEndPoint()
            buttonRow.visible = true  // 显示按钮
            captureArea = Qt.rect(selectionRect.startPoint.x, selectionRect.startPoint.y,
                                  selectionRect.width, selectionRect.height)
        }
    }

    // 实时显示的选择区域
    Rectangle {
        id: selectionRect
        color: tranparentColor
        property var control: parent
        property var startPoint
        property var endPoint

        visible: false
        border.color: selectBorderColor
        border.width: 3

        //鼠标区域，控制拖动选择框时位置变化
        MouseArea {
            id: dragItem

            anchors.fill: parent
            anchors.margins: 12 * 2
            drag.target: parent
            cursorShape: Qt.SizeAllCursor
            onPositionChanged: {
                updateStartAndEndPoint()
            }
        }

        Row {
            id: buttonRow
            spacing: 3
            visible: false
            anchors{
                top: selectionRect.bottom
                topMargin: 3
                right:selectionRect.right
            }

            Button{
                id: cancelBtn
                text: "取消"
                onClicked: {
                    initToBegin()
                    screenShotWindow.hide()
                }
            }

            Button{
                id: oKBtn
                text: "确定"
                onClicked: {
                    // 更新最终选择的区域
                    captureArea = Qt.rect(selectionRect.startPoint.x, selectionRect.startPoint.y,
                                          selectionRect.width, selectionRect.height)
                    provideArea(captureArea)
                    initToBegin()
                    screenShotWindow.hide()
                }
            }
        }

    }

    // 实时更新最终选择的区域
    function updateStartAndEndPoint() {
        selectionRect.startPoint = Qt.point(x, y);
        selectionRect.endPoint = Qt.point(x + width, y + height);
    }

    // 重新初始化
    function initToBegin(){
        selectionRect.width = 0
        selectionRect.height = 0
        selectionRect.visible = false
        buttonRow.visible = false
    }

}


