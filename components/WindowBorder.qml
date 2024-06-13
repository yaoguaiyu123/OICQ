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
                        Layout.preferredWidth: 40
                        Layout.preferredHeight: 30
                        imagePath: "qrc:/icon/minimize.png"
                        backColor: "#f2f2f2"
                        backHoverColor: "#cfcfcf"
                        onClicked: {
                            workWindow.showMinimized()
                        }
                    }
                    MyComponent.BackIconButton {
                        Layout.preferredWidth: 40
                        Layout.preferredHeight: 30
                        imagePath: "qrc:/icon/maximize.png"
                        backColor: "#f2f2f2"
                        backHoverColor: "#cfcfcf"

                        property int lastClickTime: 0
                        property int clickThreshold: 500

                        onClicked: {
                            var currentTime = new Date().getTime();
                            if (currentTime - lastClickTime > clickThreshold) {
                                lastClickTime = currentTime;
                                workWindow.toggleMaximizeRestore();
                            }
                        }
                    }
                    MyComponent.BackIconButton{
                        Layout.preferredWidth: 40
                        Layout.preferredHeight: 30
                        imagePath: "qrc:/icon/close.png"
                        imageHoverPath: "qrc:/icon/close_w.png"
                        backColor: "#f2f2f2"
                        backHoverColor: "#cc4444"
                        onClicked: {
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

}
