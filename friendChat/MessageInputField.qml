import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "../components" as MyComponent
import CustomComponents
import QtQuick.Dialogs
import CustomWindows


Rectangle {
    id: container
    height: 200
    Layout.fillWidth: true
    color: "#f2f2f2"
    signal send(string text)   //自定义信号
    signal sendFile(string text)   //自定义信号

    Row{
        x: 8
        spacing: 8
        MyComponent.IconButton{
            width: 32
            height: 32
            imagePath: "qrc:/icon/biaoqing_black.png"
            hoveredImagePath: "qrc:/icon/biaoqing_blue.png"
        }
        MyComponent.IconButton{
                    width: 32
                    height: 32
                    imagePath: "qrc:/icon/jiandao_black.png"
                    hoveredImagePath: "qrc:/icon/jiandao_blue.png"
                    onClicked: {
                        screenCapture.printFullScreen()  //获得当前全屏图片
                        screenPrintWindow.flushBackImage()
                        screenPrintWindow.show()
                        screenPrintWindow.visibility = Window.FullScreen  //使其全屏
                    }
                    MyComponent.ScreenPrint{
                        id:screenPrintWindow
                        onProvideArea:(area)=>{
                            // 得到截图位置
                            screenCapture.printScreen(area)
                        }
                    }

                    // 用于实现捕获屏幕的对象
                    ScreenCapture {
                        id: screenCapture
                        onScreenshotComplete: ()=>{
                            documentHandler.insertScreenshot()
                        }
                    }

                }
        MyComponent.IconButton{
            width: 32
            height: 32
            imagePath: "qrc:/icon/wenjian_black.png"
            hoveredImagePath: "qrc:/icon/wenjian_blue.png"
            onClicked: {
                fileDialog.open()
            }
            FileDialog {
                id: fileDialog
                title: "选择文件"
                fileMode: FileDialog.OpenFiles
                acceptLabel: "Open"
                rejectLabel: "Cancel"
                onAccepted: {
                    // console.log("用户选择的文件: " + fileDialog.selectedFile)
                    sendFile(fileDialog.selectedFiles)
                }
            }
        }
        MyComponent.IconButton{
            width: 32
            height: 32
            imagePath: "qrc:/icon/tupian_black.png"
            hoveredImagePath: "qrc:/icon/tupian_blue.png"
        }
    }

    ScrollView {
        id: view
        y: 32
        width: container.width - 20
        height: container.height - 32
        //数据缓冲的TextArea
        TextArea {
            id: hiddenArea
            textFormat: TextArea.RichText
            visible: false
            z:-1
        }

        TextArea {
            id: area
            height: 200
            width: view.width
            color: "#0E0E0E"
            font.pixelSize: 14
            textFormat: TextArea.RichText
            background: Rectangle {
                color: "#f2f2f2"
                radius: 4
            }
            verticalAlignment: TextArea.AlignTop
            wrapMode: TextArea.WrapAnywhere
            Keys.onPressed: (event) => {
                if ((event.key === Qt.Key_Return || event.key === Qt.Key_Enter) &&
                    (event.modifiers & Qt.ShiftModifier)) {
                    area.insert(area.cursorPosition, "<br/>")
                    event.accepted = true
                } else if ((event.key === Qt.Key_Return || event.key === Qt.Key_Enter)) {
                    if (!documentHandler.isTextContentEmpty()) {
                        send(area.text)    //发出信号
                        area.text = ""
                        event.accepted = true
                    }
                } else if (event.modifiers === Qt.ControlModifier && event.key === Qt.Key_V) {
                    // 自定义粘贴逻辑
                    event.accepted = true
                    hiddenArea.paste()
                    documentHandler.parseHtml()
                    hiddenArea.clear()
                    area.insert(area.cursorPosition, "<br/>")
                }
            }
            TextDocumentHandler {
                id: documentHandler
                textDocument: area.textDocument
                hideTextDocument: hiddenArea.textDocument
                cursorPosition: area.cursorPosition
            }
        }
    }


    Button{
        width: 95
        height: 24
        x: container.width - 115
        y: container.height - 45
        background: Rectangle{
            radius: 2
            color: "#0099ff"
        }
        contentItem: Text{
            text:"发送"
            color: "white"
            font.pixelSize: 15
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        onClicked: {
            if(!documentHandler.isTextContentEmpty()){
                send(area.text)    //发出信号
                area.text = ""
            }
        }

    }




}
