import QtQuick
import QtQuick.Controls

Rectangle {
    signal timerstart()  //信息过大时，点击发送会发射该信号
    signal timerend()
    id: sendrestrict
    visible: false
    z: -1
    color: "white"
    width: 220
    height: 40
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
               // running: false
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
            //sendrestrict.visible = false;
            timerend()
            anim.running = false
        }
    }

    // onVisibleChanged: {
    //     sprite2.running = true
    //     sprite1.running = true
    //     timer1.running = true
    // }

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
