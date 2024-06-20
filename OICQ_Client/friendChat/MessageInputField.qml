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
    property bool isPreparePrint:false  //是否准备截图
    property bool selectHide: false
    // signal hidecut

    Row{
        x: 8
        spacing: 8
        MyComponent.IconButton{
            width: 32
            height: 32
            imagePath: "qrc:/icon/biaoqing_black.png"
            hoveredImagePath: "qrc:/icon/biaoqing_blue.png"
            onClicked: memeview.visible = true

        }
        MyComponent.IconButton{
                    width: 32
                    height: 32
                    imagePath: "qrc:/icon/jiandao_black.png"
                    hoveredImagePath: "qrc:/icon/jiandao_blue.png"
                    onClicked: {
                        isPreparePrint=true
                        // hidecut()
                        if(selectHide===true&&isPreparePrint===true)
                        {
                         //todo hide window set window visibility equals ispreparePrint
                            rootWindow.visible=false
                        }
                        screenCapture.printFullScreen()  //获得当前全屏图片
                        screenPrintWindow.flushBackImage()
                        screenPrintWindow.show()
                        screenPrintWindow.visibility = Window.FullScreen  //使其全屏
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
                            //图片插入聊天框
                            documentHandler.insertScreenshot()
                            //截图选区结束
                            isPreparePrint=false
                        }
                    }

                }
        MyComponent.IconButton{
                   width: 16
                   height: 20
                   imagePath: "qrc:/icon/向下.png"
                   hoveredImagePath: "qrc:/icon/向下_blue.png"
                   //anchors.horizontalCenter: parent.horizontalCenter
                   anchors.verticalCenter: parent.verticalCenter
                   onClicked: {
                   //todo打开一个按钮选项菜单
                   // MyComponent.RoundCheckBox.popup()
                       checkBox.popup()
                   }
                   //todo点击菜单后隐藏qq窗口进行截图

                   MyComponent.RoundCheckBox{
                        id:checkBox
                        //按钮选择信号处理
                        onIsChecked: {
                            selectHide=true
                            //hiddenWindow()
                        }
                        onNoChecked: {
                            selectHide=false
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
    MemeView {
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
                //错误 仅调试用，消息过多的限制弹窗
                sendrestrict.timerstart()
            }
        }

    }

    //剪切操作处理器
    function handleCut() {
        console.log("Cut signal received!")
        // 在这里执行剪切操作
        area.cut()

    }

    //复制操作处理器
    function handleCopy() {
        console.log("Copy signal received!")
        // 在这里执行复制操作
        //area.selectedText.copy();
        area.copy();

    }

    //粘贴操作处理器
    function handlePaste() {
        console.log("Paste signal received!")
        // 在这里执行粘贴操作
        //area.paste()
        hiddenArea.paste()
        documentHandler.parseHtml()
        hiddenArea.clear()
        area.insert(area.cursorPosition, "<br/>")
    }

    //设置窗口隐藏操作处理器
    //判断是否点击截图且已勾选隐藏qq窗口
    function hiddenWindow(){
        console.log("Hidden the QQ window before cutScreen！")
        //todo将qq窗口隐藏
        if(selectHide===true&&isPreparePrint===true)
        {
         //todo hide window set window visibility equals ispreparePrint
            rootWindow.visible=false
        }else{
            rootWindow.visible=true
        }
    }

}
