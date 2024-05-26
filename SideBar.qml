import QtQuick
import QtQuick.Layouts
import "./components" as MyComponent

Rectangle {
    id:sideBar
    color: "red"
    Row{
        Rectangle{
            width: 60
            height:30
            color: "#f2f2f2"
        }
        Rectangle{
            width: 320
            height: 30
            color: "white"
        }
        Rectangle{
            id:buttonContainer
            width: sideBar.width - 380
            height: 30
            color: "#f2f2f2"
            Row{
                id:row
                anchors.right: buttonContainer.right
                MyComponent.BackIconButton{
                    imagePath: "qrc:/icon/minimize.png"
                    imageWidth: 20
                    imageHeight: 20
                    userButton.onClicked: {
                        rootWindow.visibility = Window.Minimized
                    }
                }
                MyComponent.BackIconButton{
                    imagePath: rootWindow.visibility ===
                               Window.Windowed ?"qrc:/icon/maximize_1.png" : "qrc:/icon/maximize.png"
                    imageWidth: 16
                    imageHeight: 16
                    userButton.onClicked: {
                        if(rootWindow.visibility === Window.Windowed){
                            rootWindow.visibility = Window.Maximized
                        }else{
                            rootWindow.visibility = Window.Windowed
                        }
                    }
                }
                MyComponent.BackIconButton{
                    imagePath: "qrc:/icon/close.png"
                    imageHoverPath: "qrc:/icon/close_w.png"
                    backHoverColor: "#ff5555"
                    imageWidth: 20
                    imageHeight: 20
                    userButton.onClicked: {
                        var currentDate = new Date();
                        var standardFormat = currentDate.toLocaleString();
                        console.log(standardFormat,"退出程序");
                        Qt.quit()
                    }
                }
            }
        }
    }
}
