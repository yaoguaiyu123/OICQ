import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components" as MyComponent

//窗口按钮组

Item {
    id:border
    implicitHeight: 50
    implicitWidth: 360
    property bool isMaxed: false
    property int lastX: 0
    property int lastY: 0
    property int lastW: 0
    property int lastH: 0

    //行布局
    RowLayout{
        id:layout
        anchors.fill: parent
        Rectangle{
            id:rec1
            Layout.fillHeight: true
            Layout.preferredWidth: 60
            color: "#f2f2f2"
        }
        Rectangle{
            id: rec2
            Layout.preferredWidth: 310
            Layout.fillHeight: true

        }
        Rectangle{
            id: rec3
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#f2f2f2"
            ColumnLayout{
                anchors.fill: parent
                RowLayout{
                    spacing: 0
                    Layout.alignment: Qt.AlignTop
                    Item{
                        height: 1
                        Layout.fillWidth: true
                    }
                    MyComponent.BackIconButton{
                        imagePath: "qrc:/icon/minimize.png"
                        backColor: "#f2f2f2"
                        backHoverColor: "#cfcfcf"
                        imageHeight: 15
                        imageWidth: 15
                        userButton.onClicked: {
                            rootWindow.showMinimized()
                        }
                    }
                    MyComponent.BackIconButton{
                        imagePath: "qrc:/icon/maximize.png"
                        backColor: "#f2f2f2"
                        backHoverColor: "#cfcfcf"
                        imageHeight: 15
                        imageWidth: 15
                        userButton.onClicked: {
                            if(isMaxed){
                                animateWindowSizeAndPosition()
                                isMaxed = false
                            }else{
                                animateWindowSizeAndPosition1()
                                isMaxed = true
                            }
                        }

                        NumberAnimation {
                            id: widthAnimation
                            target: rootWindow
                            property: "width"
                            to: lastW
                            duration: 5
                            easing.type: Easing.InOutQuad
                        }

                        NumberAnimation {
                            id: heightAnimation
                            target: rootWindow
                            property: "height"
                            to: lastH
                            duration: 5
                            easing.type: Easing.InOutQuad
                        }

                        NumberAnimation {
                            id: xAnimation
                            target: rootWindow
                            property: "x"
                            to: lastX
                            duration: 5
                            easing.type: Easing.InOutQuad
                        }

                        NumberAnimation {
                            id: yAnimation
                            target: rootWindow
                            property: "y"
                            to: lastY
                            duration: 5
                            easing.type: Easing.InOutQuad
                        }


                        NumberAnimation {
                            id: widthAnimation1
                            target: rootWindow
                            property: "width"
                            to: Screen.width
                            duration: 5
                            easing.type: Easing.InOutQuad
                        }

                        NumberAnimation {
                            id: heightAnimation1
                            target: rootWindow
                            property: "height"
                            to: Screen.height
                            duration: 5
                            easing.type: Easing.InOutQuad
                        }

                        NumberAnimation {
                            id: xAnimation1
                            target: rootWindow
                            property: "x"
                            to: 0
                            duration: 5
                            easing.type: Easing.InOutQuad
                        }

                        NumberAnimation {
                            id: yAnimation1
                            target: rootWindow
                            property: "y"
                            to: 0
                            duration: 5
                            easing.type: Easing.InOutQuad
                        }
                    }
                    MyComponent.BackIconButton{
                        imagePath: "qrc:/icon/close.png"
                        imageHoverPath: "qrc:/icon/close_w.png"
                        backColor: "#f2f2f2"
                        backHoverColor: "#cc4444"
                        imageHeight: 15
                        imageWidth: 15
                        userButton.onClicked: {
                            Qt.quit()  //关闭应用
                        }
                    }

                }
                Item{
                    width: 1
                    Layout.fillHeight: true
                }
            }

        }
    }

    function animateWindowSizeAndPosition() {

        widthAnimation.start()
        heightAnimation.start()
        xAnimation.start()
        yAnimation.start()
    }


    function animateWindowSizeAndPosition1() {
        lastH = rootWindow.height
        lastW = rootWindow.width
        lastX = rootWindow.x
        lastY = rootWindow.y
        widthAnimation1.start()
        heightAnimation1.start()
        xAnimation1.start()
        yAnimation1.start()
    }


}
