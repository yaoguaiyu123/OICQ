import QtQuick
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls
import "../components" as MyComponent
import "../comman/NetChat.js" as NetChat
import oicqclient

Rectangle {
    id:container
    color: "#f2f2f2"
    property alias inputField: _inputField
    property var dataModel : null
    property int listIndex: 0
    property string headPath: ""

    SendRestriction{
            id: sendrestrict
            visible: false
            z: 1
            anchors.horizontalCenter: parent.horizontalCenter
        }
    //列布局
    ColumnLayout{
        anchors.fill: parent
        id:layout
        MyComponent.ThreeButtons{
           // implicitWidth: 50
        }

        //行布局
        RowLayout{
            id:topRow
            width: layout.width
            Text {
                id:title
                Layout.leftMargin: 25
                font.pixelSize: 17
            }

            Item {
                Layout.fillWidth: true // 占据剩余的空间
            }

            Item{
                width: 90
                height: 30
                Layout.alignment: Qt.AlignRight
                // color: "red"
                Row{
                    spacing: 10
                    MyComponent.IconButton{
                        width: 30
                        height: 30
                        imagePath: "qrc:/icon/jia1.png"
                    }
                    MyComponent.IconButton{
                        width: 30
                        height: 30
                        imagePath: "qrc:/icon/more.png"
                    }
                }
            }
        }

        // 分割线
        Rectangle{
            color: "#dfdfdf"
            Layout.fillWidth: true
            height: 1
        }



        ListView {
            id: listView
            spacing: 30
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            model: MessageModel
            delegate: MessageDelegate {
                id: msgDelegate
                headUrl: container.headPath
                myheadUrl: FriendModel.myImagePath
                viewWidth: listView.width - 20
                x: 10
                onUploadFile1: (filepath, messageIndex) => {
                    Controller.downloadFileRequest(listIndex, messageIndex, filepath)
                }
                onCancelTransfer1: (messageIndex)=>{
                    Controller.cancelUploadOrDownload(listIndex, messageIndex)  //取消下载或者上传
                }
                onBrowsePictures: (messageIndex) => {
                    var res = Controller.currentWindowImages(listIndex, messageIndex)

                    var imageIndex = res[0]
                    var imagePaths = res.slice(1)
                    imageViewer.loadImages(imageIndex,imagePaths)
                    imageViewer.show()
                }

            }
        }




        Rectangle{
            color: "#dfdfdf"
            Layout.fillWidth: true
            height: 1
        }
        MessageInputField{
            id:_inputField
            height: 200
            onSend:(text)=>{
                // 发送消息的逻辑
                Controller.sendMessage(text,listIndex,NetChat.MSG_TYPE.PrivateMessage)
                //发送的时候也需要更新窗口滚动
                listView.positionViewAtIndex(listView.count - 1,ListView.End)
            }
            onSendFile: (text)=>{
                // console.log("将发送",text)
                Controller.sendMessage(text,listIndex,NetChat.MSG_TYPE.FileMessage)
                listView.positionViewAtIndex(listView.count - 1,ListView.End)
            }
        }
    }


    // PostSend{
    //     id:postSend
    //     //处理post请求,发送给大模型的信息
    //     onReplyFinished:(textArg,id)=> {
    //         dataModel.get(id).recode.append({
    //             date: new Date(),
    //             type: 'recv',
    //             msg: textArg
    //         })
    //         if(id === listIndex){
    //             listView.positionViewAtEnd()
    //         }
    //     }
    // }

    // MyComponent.ImageViewer{
    //     id:imageViewer
    //     visible: false
    // }

    MyComponent.PictureViewer{
        id:imageViewer
        visible: false
    }

    // MessageDialog{
    //     id: tipDialog
    //     title: qsTr("一次最多只能上传5个文件")
    //     informativeText: "请重新选择"
    //     buttons: MessageDialog.Ok
    // }

    function updateModel(index){
        listIndex = index
        Controller.updateMessageModel(listIndex)
        title.text = FriendModel.currentName
        container.headPath = FriendModel.currentHeadpath
        listView.positionViewAtEnd()   //窗口滚动到最末尾
    }

    Connections{
        target: FriendModel
        //初始化完毕之后再给qml model赋值
        function onInitDataFinished(){
            listView.model = MessageModel
            Controller.updateMessageModel(listIndex)
            title.text = FriendModel.currentName
            container.headPath = FriendModel.currentHeadpath
            listView.positionViewAtEnd()
        }
        //接收到新的消息
        function onNewMessage(index){
            if(index === listIndex){
                listView.positionViewAtIndex(listView.count - 1,ListView.End)
            }
        }
        //过多文件
        function onToManyFiles(){
            mainView.allDialogs.tipDialog.open()
        }
    }
}

