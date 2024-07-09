import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Menu{
    id:selectionmenu
    implicitWidth: 120
    implicitHeight: 91
    signal cutSignal
    signal copySignal
    signal pasteSignal
    padding: 5
    Action{
        text: qsTr("剪切")
        onTriggered: {
            cutSignal()
    }
    }
    Action{
        text: qsTr("复制")
        onTriggered: {
        copySignal()
    }
    }
    Action{
        text: qsTr("粘贴")
        onTriggered: {
        pasteSignal()
    }
    }

    delegate: MenuItem{
        id:menuItem
        implicitWidth: 120
        implicitHeight: 27
        leftInset: 10
        topInset: 5
        font.pixelSize: 15
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
        // 在这里执行剪切操作
        area.cut()

    }

    //复制操作处理器
    function handleCopy() {
        // 在这里执行复制操作
        //area.selectedText.copy();
        area.copy();

    }

    //粘贴操作处理器
    function handlePaste() {
        // 在这里执行粘贴操作
        //area.paste()
        hiddenArea.paste()
        documentHandler.parseHtml()
        hiddenArea.clear()
        area.insert(area.cursorPosition, "<br/>")
    }
}

