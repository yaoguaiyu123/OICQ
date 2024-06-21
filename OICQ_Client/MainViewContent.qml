import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "./friendChat" as FriendChat
import "components" as MyComponent

Rectangle {
    id:mainView
    property int currentIndex: 0
    property var components: ["friendChat/F1.qml",
                              "friendChat/F2.qml"]

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
                id:headPopup
                anchors.centerIn:parent
            }
        }
    }


}
