import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// 图片浏览器

Window {
    width: 720
    height: 640
    visible: true
    title: qsTr("OICQ图片浏览器")
    property int currentIndex: 0
    property var imagePaths : []
    property int num: 100 //默认比例数字
    property real imageScale: 1 // 控制图片的缩放比例
    //判断图片是否达到最大最小
    property bool isSmallist: false
    property bool isBiggest: false

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
                    scale: imageScale
                    DragHandler {
                        target: photoImage
                        xAxis.enabled: photoImage.width * photoImage.scale > flickable.width ||
                                       photoImage.height * photoImage.scale > flickable.height
                        yAxis.enabled: photoImage.width * photoImage.scale > flickable.width ||
                                       photoImage.height * photoImage.scale > flickable.height
                    }
                }

                //最大值提示框
                Rectangle{
                    id:tipsMax
                    visible: false
                    color: "transparent"
                    anchors.centerIn: parent
                    implicitWidth: 100
                    implicitHeight: 50
                    radius: 4
                    Label{
                        color: "white"
                        text: "The image has reached its maximum value ！"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 16
                        anchors.centerIn:parent
                    }
                }
                //最小值提示框
                Rectangle{
                    id:tipsMin
                    visible: false
                    color:"transparent"
                    anchors.centerIn: parent
                    implicitWidth: 100
                    implicitHeight: 50
                    radius: 4
                    Label{
                        color: "white"
                        text: "The image has reached its minimum value ！"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 16
                        anchors.centerIn:parent
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
                        text: num+" %"
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
                        color: parent.hovered ? Qt.rgba(30, 30, 30, 0.6) : Qt.rgba(30, 30, 30, 0.3)
                    }

                    contentItem: Label {
                        text: "<"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 25
                        color: "#000000"
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
                        color: parent.hovered ? Qt.rgba(30, 30, 30, 0.6) : Qt.rgba(30, 30, 30, 0.3)
                    }

                    contentItem: Label {
                        text: ">"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 25
                        color: "#000000"
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
                            if (wheel.angleDelta.y > 0 && imageScale<10) {
                                // 放大
                               //photoImage.scale *= (wheel.angleDelta.y / 600 + 1)
                                        imageScale*=(wheel.angleDelta.y / 600 + 1)
                                        num=parseInt(imageScale*100)
                                        if(num>=1000){
                                            isBiggest=true
                                            tipsMax.visible=true
                                            num=1000
                                        }else if(num>=10){
                                            isSmallist=false
                                            tips.visible=true
                                        }
                                        closeTimer.start()
                            } else if (wheel.angleDelta.y < 0 && imageScale>0.1) {
                                // 缩小
                               imageScale *= (wheel.angleDelta.y / 600 + 1)
                                        num=parseInt(imageScale*100)
                                        if(num<=10){
                                            isSmallist=true
                                            tipsMin.visible=true
                                            num=10
                                        }
                                        else if(num<=1000){
                                        isBiggest=false
                                        tips.visible=true
                                        }

                                        closeTimer.start()
                                if(imageScale < 1.2){
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
        Rectangle{
            id:background
            color: "black"
            Layout.preferredHeight: 50
            Layout.fillWidth: true
            RowLayout{
                anchors.centerIn: parent
                spacing: 30
                //减小图片大小的按钮
                IconButton {
                        id:smaller
                        backColor: background.color
                        imagePath:  "qrc:/icon/详情页放大镜图标减.png"
                        enabled: !isSmallist
                        onClicked: {
                            imageScale/= 1.2 ;// 减少20%
                            num=parseInt(num/1.2);
                            if(num<=10){
                                isSmallist=true
                                tipsMin.visible=true
                                num=10
                            }
                            else if(num<=1000){
                            isBiggest=false
                            tips.visible=true
                            }

                            closeTimer.start()
                        }
                    }

                    //显示百分比
                    Text{
                        id:numText
                        text:num+" %"
                        //anchors.centerIn:background
                        color:"white"
                        font.pixelSize: 25
                        font.family: "微软雅黑"
                    }

                  //增加图片大小的按钮
                    IconButton {
                        id:bigger
                        backColor: background.color
                        imagePath:  "qrc:/icon/详情页放大镜图标加.png"
                        enabled: !isBiggest
                        onClicked: {
                            imageScale *= 1.2 ;// 增加20%
                             num=parseInt(num*1.2);
                            if(num>=1000){
                                isBiggest=true
                                tipsMax.visible=true
                                num=1000
                            }else if(num>=10){
                        isSmallist=false
                        tips.visible=true
                            }
                            closeTimer.start()
                        }
                    }

                }
        }
    }

    //控制器，控制tips提示时间
    Timer {
        id: closeTimer
        interval: 1000  // 设置定时器时间为1000毫秒（1秒）
        onTriggered: {
            tips.visible = false
            tipsMax.visible=false
            tipsMin.visible=false
        }

    }

    function loadImages(index, images){
        currentIndex = index
        imagePaths = images
        photoImage.source = (imagePaths === undefined ? "" : imagePaths[currentIndex])
    }

}

