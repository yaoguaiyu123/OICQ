import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// 图片浏览器

Window {
    width: 720
    height: 640
    visible: true
    title: qsTr("仿QQ图片浏览器")
    property int currentIndex: 0
    property var imagePaths : []

    ColumnLayout{
        anchors.fill: parent
        spacing: 0

        // 图片浏览区域
        Control {
            Layout.fillHeight: true
            Layout.fillWidth: true
            background: Rectangle {
                color: "#1b1b1b"
            }

            contentItem: Flickable {
                id: flickable

                Image {
                    id: photoImage
                    fillMode: Image.PreserveAspectFit
                    source: ""
                    width: flickable.width / 1.5
                    x: parent.width / 2 - width / 2
                    y: parent.height / 2 - height / 2

                    SequentialAnimation {
                        id: moveToCenterAnimation
                        NumberAnimation {
                            target: photoImage
                            property: "x"
                            to: flickable.width / 2 - photoImage.width / 2
                            duration: 260
                            easing.type: Easing.OutQuad
                        }
                        NumberAnimation {
                            target: photoImage
                            property: "y"
                            to: flickable.height / 2 - photoImage.height / 2
                            duration: 260
                            easing.type: Easing.OutQuad
                        }
                    }

                    DragHandler {
                        target: photoImage
                        xAxis.enabled: photoImage.width * photoImage.scale > flickable.width ||
                                       photoImage.height * photoImage.scale > flickable.height
                        yAxis.enabled: photoImage.width * photoImage.scale > flickable.width ||
                                       photoImage.height * photoImage.scale > flickable.height
                    }
                }


                // 提示框
                Rectangle{
                    id:tips
                    visible: false
                    color: Qt.rgba(0,0,0,0.6)
                    anchors.centerIn: parent
                    implicitWidth: 100
                    implicitHeight: 50
                    radius: 4
                    Label{
                        color: "white"
                        text: "100%"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 16
                        anchors.centerIn:parent
                    }
                }

                // 左按钮
                Button {
                    implicitWidth: 60
                    implicitHeight: 60
                    anchors.verticalCenter: parent.verticalCenter
                    hoverEnabled: true
                    flat: true
                    highlighted: true

                    background: Rectangle {
                        radius: width / 2
                        color: parent.hovered ? Qt.rgba(0, 0, 0, 0.6) : Qt.rgba(0, 0, 0, 0.4)
                    }

                    contentItem: Label {
                        text: "<"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 25
                    }

                    onPressed: {
                        if(currentIndex > 0){
                            currentIndex--
                            photoImage.source = imagePaths[currentIndex]
                        }
                    }
                }

                // 右按钮
                Button {
                    implicitWidth: 60
                    implicitHeight: 60
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    hoverEnabled: true
                    flat: true
                    highlighted: true

                    background: Rectangle {
                        radius: width / 2
                        color: parent.hovered ? Qt.rgba(0, 0, 0, 0.6) : Qt.rgba(0, 0, 0, 0.4)
                    }

                    contentItem: Label {
                        text: ">"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 25
                    }

                    onPressed: {
                        if(currentIndex < imagePaths.length - 1){
                            currentIndex++
                            photoImage.source = imagePaths[currentIndex]
                        }
                    }
                }


                WheelHandler {
                    onWheel:(wheel)=> {
                        if(wheel.modifiers & Qt.ControlModifier){
                            photoImage.rotation += wheel.angleDelta.y / 12
                            if(Math.abs(Math.abs(photoImage.rotation) % 360 - 360) < 5){
                                photoImage.rotation = 0
                            }
                            wheel.accepted = true
                        }else{
                            if (wheel.angleDelta.y > 0 && photoImage.scale < 6) {
                                // 放大
                                photoImage.scale *= (wheel.angleDelta.y / 600 + 1)
                            } else if (wheel.angleDelta.y < 0 && photoImage.scale > 0.3) {
                                // 缩小
                                photoImage.scale *= (wheel.angleDelta.y / 600 + 1)
                                if(photoImage.scale < 1.2){
                                    moveToCenterAnimation.start()
                                }
                            }
                            wheel.accepted = true
                        }
                    }
                }
            }
        }

        //按钮区域
        ColumnLayout{
            Layout.fillWidth: true
            Rectangle{
                color: "black"
                Layout.preferredHeight: 50
                Layout.fillWidth: true
            }
        }
    }

    function loadImages(index, images){
        currentIndex = index
        imagePaths = images
        // console.log(images)
        // console.log(imagePaths)
        photoImage.source = (imagePaths === undefined ? "" : imagePaths[currentIndex])
    }

}
