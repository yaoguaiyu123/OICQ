import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Window 2.14

// 截屏窗口

Window {
    id:screenShotWindow
    property int borderMargin: 3
    property rect captureArea: Qt.rect(0, 0, 0, 0) // 记录最终选择的范围
    signal provideArea(var area)
    property color blurryColor: Qt.rgba(0,0,0,0.3)
    color: "transparent"
    flags: Qt.FramelessWindowHint // 取消默认标题栏

    //将截屏窗口的宽和高设置为电脑屏幕宽和高，避免main窗口隐藏后出现的bug
       width: Screen.width
       height: Screen.height

    Image {
        id:backImage
        anchors.fill: parent
        source: "file:///root/.config/OICQ/client/temp/fullscreen_shot_temp.jpg"
    }

    Rectangle{
        id: topRect
        anchors{
            top: parent.top
            left: parent.left
        }
        width: parent.width
        height: selectionRect.y
        color: blurryColor
    }

    Rectangle{
        id: leftRect
        anchors{
            top: topRect.bottom
            left: parent.left
        }
        width: selectionRect.x
        height: parent.height - topRect.height - bottomRect.height
        color: blurryColor
    }

    Rectangle{
        id: rightRect
        anchors{
            top: topRect.bottom
            right: parent.right
        }
        width: parent.width - leftRect.width - selectionRect.width
        height: leftRect.height
        color: blurryColor
    }

    Rectangle{
        id: bottomRect
        anchors{
            bottom: parent.bottom
            left: parent.left
        }
        width: parent.width
        height: parent.height - topRect.height - selectionRect.height
        color: blurryColor
    }



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
            updateStartPoint()
            buttonRow.visible = true  // 显示按钮
            captureArea = Qt.rect(selectionRect.startPoint.x, selectionRect.startPoint.y,
                                  selectionRect.width, selectionRect.height)
        }
    }


    // 实时显示的选择区域
    Rectangle {
        id: selectionRect
        color: "transparent"
        property var control: parent
        property var startPoint

        visible: false
        border.color: "#0099ff"
        border.width: 2

        //鼠标区域，控制拖动选择框时位置变化
        // MouseArea {
        //     id: dragItem

        //     anchors.fill: parent
        //     anchors.margins: 12 * 2
        //     drag.target: parent
        //     cursorShape: Qt.SizeAllCursor
        //     onPositionChanged: {
        //         updateStartPoint()
        //     }
        // }

        //MouseArea修改为handler版本
        DragHandler{
            target: selectionRect
            onActiveChanged: {
                if(Drag.active)
                {
                    updateStartPoint()
                }
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
                implicitWidth: 65
                background: Rectangle{
                    color: "white"
                    radius: 5
                }
                contentItem: Label{
                    color: "black"
                    text: "取消"
                    horizontalAlignment: Text.AlignHCenter
                }
                onClicked: {
                    initToBegin()
                    screenShotWindow.hide()
                    isPreparePrint=false
                    // workWindow.show()
                }
            }

            Button{
                id: oKBtn
                implicitWidth: 65
                background: Rectangle{
                    color: "#0099ff"
                    radius: 5
                }
                contentItem: Label{
                    color: "white"
                    text: "确定"
                    horizontalAlignment: Text.AlignHCenter

                }
                onClicked: {
                    // 更新最终选择的区域
                    captureArea = Qt.rect(selectionRect.startPoint.x, selectionRect.startPoint.y,
                                          selectionRect.width, selectionRect.height)
                    provideArea(captureArea)
                    initToBegin()
                    screenShotWindow.hide()
                    isPreparePrint=false
                    // workWindow.show()
                }
            }
        }

    }

    // 实时更新selectionRectangle的左上角位置
    function updateStartPoint() {
        selectionRect.startPoint = Qt.point(selectionRect.x, selectionRect.y);
    }

    // 重新初始化
    function initToBegin(){
        selectionRect.width = 0
        selectionRect.height = 0
        selectionRect.visible = false
        buttonRow.visible = false
    }

    // 刷新背景图片
    function flushBackImage(){
        backImage.source = ""
        backImage.source = "file:///root/.config/OICQ/client/temp/fullscreen_shot_temp.jpg"
    }

}
