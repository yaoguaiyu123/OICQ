import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id:container
    //行布局
    RowLayout{
        id:layout
        anchors.fill: parent
        Rectangle{
            id:rec1
            Layout.fillHeight: true
            width: 55
            color: "#f2f2f2"
        }
        Rectangle{
            id: rec2
            width: 320
            Layout.fillHeight: true

        }
        Rectangle{
            id: rec3
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#f2f2f2"
        }
    }
}
