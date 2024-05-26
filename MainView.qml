import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "./comman/Comman.js" as Comman
import "./friendChat" as FriendChat
import "components" as MyComponent

Rectangle {
    id:mainView
    property int currentIndex: 0
    property var components: ["friendChat/FriendChatWindow.qml",
                              "friendChat/FriendAddWindow.qml"]
    // 行布局
    RowLayout{
        id:mainLayout
        anchors.fill: parent
        SideMenuBar{
            onMenuBtnClicked:(index)=>{
                if(index <= 1){
                    currentIndex = index
                    windowloader.source = components[currentIndex]
                }
            }
            onHeadPopupOpen: {
                headPopup.open()
            }
        }

        Loader {
            id:windowloader
            active: true
            Layout.fillHeight: true
            Layout.fillWidth: true
            source:components[currentIndex]
        }

        MyComponent.HeadUpLoadPopup{
            id:headPopup
            anchors.centerIn:parent
        }
    }
}
