import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

//最小化、最大化、关闭的按钮
Rectangle{
    color: "#f2f2f2"
    implicitWidth: 130
    implicitHeight: 32
    Layout.alignment: Qt.AlignRight


    RowLayout{
        spacing: 0
        Layout.alignment: Qt.AlignTop
        BackIconButton{
            Layout.preferredWidth: 40
            Layout.preferredHeight: 30
            imagePath: "qrc:/icon/minimize.png"
            backColor: "#f2f2f2"
            backHoverColor: "#cfcfcf"
            onClicked: {
                rootWindow.showMinimized()
            }
        }
        BackIconButton {
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
                    rootWindow.toggleMaximizeRestore();
                }
            }
        }
        BackIconButton{
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
}


