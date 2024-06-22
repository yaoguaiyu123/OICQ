import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "./components" as MyComponent
import oicqclient

Rectangle {
    id:container
    Layout.fillHeight:true
    implicitWidth: 60
    color: "#f2f2f2"
    signal menuBtnClicked(int index)
    signal headPopupOpen()
    property int currentIndex: 0
    ListModel{
        id:buttonModel1
        ListElement{
            path:"qrc:/icon/chat"
        }
        ListElement{
            path:"qrc:/icon/person"
        }
        ListElement{
            path:"qrc:/icon/qqKongjian"
        }
        ListElement{
            path:"qrc:/icon/qqPingdao"
        }
        ListElement{
            path:"qrc:/icon/qqShiping"
        }
        ListElement{
            path:"qrc:/icon/wendang"
        }
        ListElement{
            path:"qrc:/icon/shezhi"
        }
    }

    Column{
        y:30
        anchors.horizontalCenter: container.horizontalCenter
        spacing: 15
        MyComponent.HeadImage{
            id:head
            headUrl:Controller.getFriendModel().myImagePath
            width: 36
            height: 36
            imageRadius: 18
            TapHandler{
                onTapped: headPopupOpen()  //发出信号
            }
        }

        Repeater{
            id:repeater1
            model: buttonModel1
            //这边的currentIndex只是用来对当前选择的项进行ui切换
            MyComponent.IconButton{
                width: 31
                height: 31
                imagePath: path + (currentIndex === index ? "_blue" :"_black")
                backColor: currentIndex === index ? "#e8e8e8" : "#f2f2f2"
                onClicked: {
                    currentIndex = index
                    //发出信号
                    container.menuBtnClicked(index)
                }
            }
        }
    }

}
