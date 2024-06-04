import QtQuick
import QtQuick.Layouts
import "../components" as MyComponent
import "../comman/Comman.js" as Comman
import "../comman/NetChat.js" as NetChat
import NetWorks
import CustomModels

Rectangle {
    id:container
    color: "#f2f2f2"
    property var dataModel : null
    property int listIndex: 0
    property string headPath: FriendModel.currentHeadpath
    property string url: "https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/chat/completions?access_token=24.e242c2f0ea3971c6f72f402ed63b7150.2592000.1717236204.282335-66121946"
    //列布局
    ColumnLayout{
        anchors.fill: parent
        id:layout
        //行布局
        RowLayout{
            id:topRow
            width: layout.width
            Text {
                id:title
                Layout.leftMargin: 25
                //TODO
                text: FriendModel.currentName
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
        Rectangle{
            color: "#dfdfdf"
            Layout.fillWidth: true
            height: 1
        }
        ListView{
            id:listView
            spacing: 30
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            delegate: MessageDelegate{
                id:msgDelegate
                headUrl: container.headPath
                viewWidth: listView.width - 40  //传入宽度
                x: listView.width / 2 - viewWidth / 2   //居中
            }
        }
        Rectangle{
            color: "#dfdfdf"
            Layout.fillWidth: true
            height: 1
        }
        MessageInputField{
            height: 200
            onSend:(text)=>{
                // 发送消息的逻辑
                FriendModel.sendMessage(text,listIndex,NetChat.MSG_TYPE.PrivateMessage)
                //发送的时候也需要更新窗口滚动
                listView.positionViewAtIndex(listView.count - 1,ListView.Beginning)
            }
            onSendFile: (text)=>{
                FriendModel.sendMessage(text,listIndex,NetChat.MSG_TYPE.FileMessage)
                listView.positionViewAtIndex(listView.count - 1,ListView.Beginning)
            }
        }
    }


    PostSend{
        id:postSend
        //处理post请求,发送给大模型的信息
        onReplyFinished:(textArg,id)=> {
            dataModel.get(id).recode.append({
                date: new Date(),
                type: 'recv',
                msg: textArg
            })
            if(id === listIndex){
                listView.positionViewAtIndex(listView.count - 1,ListView.Beginning)
            }
        }
    }

    function updateModel(index){
        listIndex = index
        listView.model = FriendModel.getMessageModel(index)
        listView.positionViewAtEnd()   //窗口滚动到最末尾
    }

    Connections{
        target: FriendModel
        //初始化完毕之后再给qml model赋值
        function onInitDataFinished(){
            // qDebug() <<
            listView.model = FriendModel.getMessageModel()
        }
        //接收到新的消息
        function onNewMessage(index){
            if(index === listIndex){
                listView.positionViewAtIndex(listView.count - 1,ListView.Beginning) ;
            }else{
                //TODO
            }
        }
    }

}

