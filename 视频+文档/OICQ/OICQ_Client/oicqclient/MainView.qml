import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "./friendChat" as FriendChat
import "components" as MyComponent

Rectangle {
    id:mainView
    property alias headPopup: _headPopup
    property alias allDialogs: dialogs
    property alias windowLoader: windowloader
    property int currentIndex: 0
    property var components: ["friendChat/FriendChatWindow.qml",
                              "friendChat/FriendAddWindow.qml"]

    ColumnLayout{
        anchors.fill: parent
        spacing: 0

        // 行布局
        RowLayout{
            id:mainLayout
            Layout.fillHeight: true
            Layout.fillWidth: true
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
                Layout.fillHeight: true
                Layout.fillWidth: true
                source:components[currentIndex]
            }

            MyComponent.HeadUpLoadPopup{
                id:_headPopup
                anchors.centerIn:parent
            }
        }
    }

    Dialogs{
        id:dialogs
    }
}
