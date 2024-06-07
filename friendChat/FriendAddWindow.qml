import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "../components" as MyComponent
import CustomModels
//好友添加消息的窗口
Item {
    property bool buttonClicked: false  // 定义一个属性来记录按钮是否被点击
    id:friendAddWindow
    //监听
    onWidthChanged: {
    if(width<870){
        view.visible=false;
    }else if(buttonClicked==true){
        view.visible=true;
    }
    }
    //行布局
    RowLayout{
        anchors.fill: parent
        // 导航
        Rectangle{
            id:leftRectangle
            Layout.preferredWidth: 310
            Layout.fillHeight: true
            FriendsGroupMenu{
                id:friendsGroupMenu
                x: addButton.x + addButton.width
                y: addButton.y + addButton.height / 2
                onOpenFgPop: {
                    fgPopup.open()
                }
            }

            FriendsGroupPopup{
                id:fgPopup
                x: friendAddWindow.width / 2 - width / 2
                y: friendAddWindow.height / 2 - height / 2
            }
            //行布局
            Row{
                id:topRow
                spacing: 15
                x: 15
                MyComponent.SearchBox{
                    width: 245
                }
                MyComponent.IconButton{
                    id:addButton
                    imagePath: "qrc:/icon/jia.png"
                    backColor: "#f2f2f2"
                    hoveredBackColor: "#dcdcdc"
                    height: 30
                    width: 30
                    onClicked: {
                        friendsGroupMenu.open()
                    }
                }
                z:2
            }
            ListModel{
                id:buttonListModel
                ListElement{ btnname :"好友通知"}
                ListElement{ btnname :"群通知"}
            }

            ListView{
                anchors.top: topRow.bottom
                anchors.topMargin: 15
                anchors.bottom: parent.bottom
                width: parent.width
                model:buttonListModel
                delegate: Button{
                    width: parent.width
                    height: 35
                    background: Rectangle{
                        id:back
                        color: "white"
                        HoverHandler{
                            onHoveredChanged: {
                                back.color = hovered ? "#f2f2f2" : "white"
                            }
                        }
                    }
                    contentItem: Text{
                        text: btnname
                        font.pixelSize: 14
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: {
                        showList(index);
                        buttonClicked=true
                    }
                }
            }

        }


        //添加好友请求列表
        Rectangle{
            id: friendRequestList
            color: "#f2f2f2"
            Layout.fillHeight: true
            Layout.fillWidth: true
            Image{
                id:backImage
                width: 120
                height: 120
                source: "qrc:/image/back.png"
                anchors.centerIn: parent
            }
            Text {
                   id:title
                   text: qsTr("好友通知")
                   font.pixelSize: 18
                   x: 15
                   y: 15
            }
            ListView{
                id:view
                visible: false
                width: parent.width
                boundsBehavior: Flickable.StopAtBounds // 到达边界时停止滚动
                anchors.left:leftRectangle.right
                anchors.top:title.bottom
                anchors.topMargin: 15
                anchors.bottom: parent.bottom
                model: FriendRequestModel
                spacing: 20
                delegate: Item{
                    id:item
                    width: parent.width
                    Rectangle{
                        height: 65
                        width: 560
                        anchors.horizontalCenter: parent.horizontalCenter
                        //x: parent.width / 2 - width / 2
                        radius: 8
                        Row{
                            x: 20
                            anchors.verticalCenter: parent.verticalCenter
                            MyComponent.HeadImage {
                                id: head
                                headUrl: headpath
                                width:45
                                height: 45
                                radius: 22.5
                            }
                            Rectangle{
                                width: 10
                                height: 5
                            }
                            Text {
                                text: name
                                color: "#0099ff"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            Rectangle{
                                width: 5
                                height: 5
                            }
                            Text {
                                text: qsTr("请求添加为好友")
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                        Button{
                            id:noButton
                            width: 40
                            height: 30
                            anchors.right: okButton.left
                            anchors.rightMargin: 20
                            anchors.verticalCenter: parent.verticalCenter
                            background: Rectangle{
                                radius: 5
                                color: "#f2f2f2"
                            }
                            contentItem: Text{
                                text: "拒绝"
                                color: "black"
                                font.pixelSize: 15
                                font.family: "微软雅黑"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            onClicked: {
                                onClicked: FriendRequestModel.choseAddFriend(0,index)
                            }
                        }
                        Button{
                            id:okButton
                            width: 40
                            height: 30
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            anchors.rightMargin: 20
                            background: Rectangle{
                                radius: 5
                                color: "#0099ff"
                            }
                            contentItem: Text{
                                text: "同意"
                                color: "white"
                                font.pixelSize: 15
                                font.family: "微软雅黑"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            onClicked: FriendRequestModel.choseAddFriend(1,index)
                        }
                    }

                }
          }
        }
    }


    function showList(index){
        if(index === 0){
            view.visible = true
            backImage.visible = false
        }
    }
}

