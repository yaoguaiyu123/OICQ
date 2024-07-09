import QtQuick
import QtQuick.Controls

// 聊天输入框为空时的提示弹窗
Rectangle {
    id: sendrestrict
    signal timerstart()
    signal timerend()

    visible: false
    z: -1
    color: "white"
    implicitWidth: 220
    implicitHeight: 40
    radius: 10
    opacity: 0  //不透明度
    Text {
        id: name
        text: qsTr("发送内容为空")
        font.pixelSize: 16
        color: "#555"
        anchors.centerIn: parent
    }

    SequentialAnimation{
        id: anim
        running: false
        ParallelAnimation {
            NumberAnimation {
                id: sprite1
                target: sendrestrict
                property: "y"
                from: 0
                to: 30
                duration: 300
            }
           PropertyAnimation{
               id: sprite2
               target: sendrestrict
               property: "opacity"
               from: 0
               to: 1
               duration: 1000
           }
        }
    }
    Timer{
        id: timer1  //计时器
        interval: 1600
        running: false
        onTriggered:{
            timerend()
            anim.running = false
        }
    }

    onTimerstart:{
        visible =true
        anim.running = true
        timer1.restart()    //每次点击都重新计时
    }

    onTimerend:  PropertyAnimation{
        id: sprite3
        running: true
        target: sendrestrict
        property: "opacity"
        from: 1
        to: 0
        duration: 800
    }

}
