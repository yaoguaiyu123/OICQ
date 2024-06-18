import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Menu{
    id:selectionmenu
    implicitWidth: 120
    implicitHeight: 120
    signal cutSignal
    signal copySignal
    signal pasteSignal
    Action{
        text: qsTr("剪切")
        onTriggered: {
            cutSignal()
        console.log("----cut----");
    }
    }
    Action{
        text: qsTr("复制")
        onTriggered: {
        copySignal()
        console.log("----copy----");
    }
    }
    Action{
        text: qsTr("粘贴")
        onTriggered: {
        pasteSignal()
        console.log("----paste----");
    }
    }

    delegate: MenuItem{
        id:menuItem
        implicitWidth: 120
        implicitHeight: 40
        leftInset: 10
        topInset: 5
        font: Qt.font({
                      family:"微软雅黑",
                      pixelSize:18
                      })
        contentItem: Text {
                    text: menuItem.text
                    font: menuItem.font
                    opacity: enabled ? 1.0 : 0.3
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
        background: Rectangle{
            anchors.centerIn: parent
            implicitHeight: 30
            implicitWidth: 100
            opacity: enabled ? 1 : 0.3
            color:menuItem.highlighted?"#e6e6e6" : "white"
            radius: 5
        }
    }
    background: Rectangle{
        implicitHeight: 30
        implicitWidth: 100
        color: "white"
        radius: 3
        anchors.fill: parent
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
}

