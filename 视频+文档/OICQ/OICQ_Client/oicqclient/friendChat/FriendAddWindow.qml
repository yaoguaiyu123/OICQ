import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "../components" as MyComponent
import oicqclient

//该窗口为联系人列表窗口
Item {
    id:friendAddWindow
    anchors.fill: parent
    property bool buttonClicked: false  // 定义一个属性来记录按钮是否被点击
    onWidthChanged: {
        if(width<870){
            view.visible=false;
        }else if(buttonClicked==true){
            view.visible=true;
        }
    }

    SplitView {
        id: split1
        anchors.fill: parent
        orientation: Qt.Horizontal
        handle: splitHandle

        SplitView {
            id: leftView
            SplitView.preferredWidth: 310
            SplitView.maximumWidth: 310
            SplitView.minimumWidth: 160

            Rectangle{
                id:leftRectangle
                anchors.fill: parent
                FriendsGroupMenu{   //点击加号之后加载的菜单
                    id:friendsGroupMenu
                    x: addButton.x + addButton.width
                    y: addButton.y + addButton.height / 2
                    onOpenFgPop: {
                        fgPopup.open()
                    }
                }

                FriendsGroupPopup{  //添加好友的弹框
                    id:fgPopup
                    x: friendAddWindow.width / 2 - width / 2
                    y: friendAddWindow.height / 2 - height / 2
                }


                RowLayout{
                    id:topRow
                    spacing: 0
                    MyComponent.SearchBox{
                        Layout.preferredWidth: 250 - (310-leftView.width)
                        Layout.topMargin: 30
                        Layout.leftMargin: 12
                    }
                    MyComponent.IconButton{
                        id:addButton
                        imagePath: "qrc:/icon/jia.png"
                        backColor: "#f2f2f2"
                        hoveredBackColor: "#dcdcdc"
                        Layout.leftMargin: 10
                        Layout.topMargin: 30
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: 30
                        onClicked: {
                            friendsGroupMenu.open()
                        }
                    }

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
        }

        SplitView {
            id: rightView
            SplitView.fillWidth: true
            Rectangle{
                id: friendRequestList
                color: "#f2f2f2"
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                ColumnLayout{
                    width: parent.width
                    MyComponent.ThreeButtons{
                        id: tb
                        Layout.alignment: Qt.AlignRight

                    }

                    Text{
                        id: title
                        text:qsTr("好友通知")
                        font.pixelSize:18
                        Layout.leftMargin: 30
                        Layout.topMargin: 20

                     }
                    Image{
                        id:backImage
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: 120
                        Layout.topMargin: 200
                        source: "qrc:/image/back.png"
                        Layout.alignment: Qt.AlignCenter

                    }
                    ListView{
                        id:view
                        visible:false
                        width:parent.width
                        boundsBehavior: Flickable.StopAtBounds      // 到达边界时listview停止滚动
                        Layout.topMargin: 15
                        Layout.preferredHeight: 600
                        spacing: 80
                        delegate: Item{
                            id:item
                            width: parent.width
                            Rectangle{
                                height: 65
                                width: 560
                                anchors.horizontalCenter: parent.horizontalCenter
                                radius: 8
                                RowLayout{
                                    height: parent.height
                                    Layout.alignment: Qt.AlignVCenter
                                    MyComponent.HeadImage{
                                        id:head
                                        headUrl:headpath
                                        Layout.leftMargin: 20
                                        Layout.preferredWidth: 45
                                        Layout.preferredHeight: 45
                                        radius:22.5
                                     }
                                    Text{
                                        text:name
                                        color:"#0099ff"
                                        Layout.alignment: Qt.AlignVCenter
                                        Layout.leftMargin: 10
                                    }
                                    Text {
                                        text: qsTr("请求添加为好友")
                                         Layout.alignment: Qt.AlignVCenter
                                        Layout.leftMargin: 5
                                    }
                                    Button{
                                        id:noButton
                                        Layout.preferredWidth: 40
                                        Layout.preferredHeight: 30
                                        Layout.leftMargin: 180
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
                                            onClicked: Controller.choseAddFriend(0,index)
                                        }
                                    }
                                    Button{
                                        id:okButton
                                        Layout.preferredWidth: 40
                                        Layout.preferredHeight: 30
                                        Layout.leftMargin: 10
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
                                        onClicked: Controller.choseAddFriend(1,index)
                                    }
                                }

                            }

                        }
                    }
                }


            }
        }

        Component {
            id: splitHandle
            Rectangle {
                implicitWidth: 4
                implicitHeight: 4
                opacity: 0
            }
        }
    }

    function showList(index){
        if(index === 0){
            view.visible = true
            view.model = FriendRequestModel
            backImage.visible = false
        }
    }
}
