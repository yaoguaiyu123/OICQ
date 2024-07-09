import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "../components" as MyComponent
import QtQuick.Dialogs
import oicqclient

//好友聊天窗口的输入框
Rectangle {
    id: container
    height: 200
    Layout.fillWidth: true
    color: "#f2f2f2"
    signal send(string text)
    signal sendFile(string text)
    property bool isPreparePrint:false  //是否准备截图
    property bool selectHide: false
    property string filePath: ""

    RowLayout{
        spacing: 8
        MyComponent.IconButton{
            Layout.preferredWidth: 32
            Layout.preferredHeight: 32
            imagePath: "qrc:/icon/biaoqing_black.png"
            hoveredImagePath: "qrc:/icon/biaoqing_blue.png"
            onClicked: {
                memeview.z = 1
                memeview.visible = true
            }
        }

        RowLayout{
            spacing: 2
            MyComponent.IconButton{
                Layout.preferredWidth: 32
                Layout.preferredHeight: 32
                imagePath: "qrc:/icon/jiandao_black.png"
                hoveredImagePath: "qrc:/icon/jiandao_blue.png"
                onClicked: {
                    isPreparePrint=true
                    if(selectHide===true && isPreparePrint===true) {
                        rootWindow.visible=false
                    }
                    timeControl.start()
                }
                MyComponent.ScreenPrint{
                    id:screenPrintWindow
                    onProvideArea:(area)=>{
                        rootWindow.visible=true
                        // 得到截图位置
                        screenCapture.printScreen(area)
                    }
                }

                // 用于实现捕获屏幕的对象
                ScreenCapture {
                    id: screenCapture
                    onScreenshotComplete: ()=>{
                        documentHandler.insertScreenshot()
                        isPreparePrint=false
                    }
                }

                Timer{
                    id:timeControl
                    interval: 250
                    onTriggered: {
                        screenCapture.printFullScreen()  //获得当前全屏图片
                        screenPrintWindow.flushBackImage()
                        screenPrintWindow.show()
                        screenPrintWindow.visibility = Window.FullScreen  //使其全屏
                    }
                }
            }
            MyComponent.IconButton{
                Layout.preferredWidth: 16
                Layout.preferredHeight:20
               imagePath: "qrc:/icon/向下.png"
               hoveredImagePath: "qrc:/icon/向下_blue.png"
               onClicked: {
                   checkBox.popup()
               }
           }
        }


        MyComponent.IconButton{
            Layout.preferredWidth: 32
            Layout.preferredHeight: 32
            imagePath: "qrc:/icon/wenjian_black.png"
            hoveredImagePath: "qrc:/icon/wenjian_blue.png"
            onClicked: {
                mainView.allDialogs.uploadFileDialog.open()
            }
        }
        MyComponent.IconButton{
            Layout.preferredWidth: 32
            Layout.preferredHeight: 32
            imagePath: "qrc:/icon/tupian_black.png"
            hoveredImagePath: "qrc:/icon/tupian_blue.png"

        }
    }


    Image {     //截图后图片的路径
        id: picture
        source: filePath
    }

    MyComponent.RoundCheckBox{      //截图时选择是否隐藏当前窗口
         id:checkBox
         onIsChecked: {
             selectHide=true
         }
         onNoChecked: {
             selectHide=false
         }
    }

    MemeView {      //表情弹窗
        id: memeview
        clip: false
         x:   8
         y:  -310
         // 接受到表情信号
         onEmojiInsert:(type)=> {
             hiddenArea.text += memeview.memepath
             documentHandler.parseHtmlWithEmoji(type)
             hiddenArea.clear()
             if(type !== 1){
                 area.insert(area.cursorPosition, "<br/>")
             }else{
                 area.insert(area.cursorPosition, "&nbsp;")
             }
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
            //处理右击菜单
             TapHandler {
                 acceptedButtons: Qt.RightButton

                 onTapped: {
                         sender.popup();
                }
            }
             MyComponent.RightPopUpMenu {
                 id: sender
                 onCutSignal: handleCut()
                 onCopySignal: handleCopy()
                 onPasteSignal: handlePaste()
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
            } else {
                sendrestrict.timerstart()
            }
        }

    }

    //剪切操作
    function handleCut() {
        console.log("Cut signal received!")
        area.cut()

    }

    //复制操作
    function handleCopy() {
        console.log("Copy signal received!")
        area.copy();

    }

    //粘贴操作
    function handlePaste() {
        console.log("Paste signal received!")
        hiddenArea.paste()
        documentHandler.parseHtml()
        hiddenArea.clear()
        area.insert(area.cursorPosition, "<br/>")
    }

}

