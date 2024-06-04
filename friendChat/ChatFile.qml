import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Control{
    id: all
    property string fileName: "example.txt"
    property string fileSize: "15.6KB"
    implicitWidth: 220
    implicitHeight: 80
    padding: 12
    background: Rectangle{
        radius: 10
        color: "white"
    }
    contentItem: RowLayout{
        ColumnLayout{
            Layout.alignment: Qt.AlignTop
            //文件名字
            Layout.fillHeight: true
            Layout.fillWidth: true
            Text {
                id: filename
                font.pixelSize: 14
                text: fileName
                color: "black"
                Layout.fillWidth: true

            }
            Item{
                width: 10
                Layout.fillHeight: true
            }

            //文件信息(大小 and 状态)
            RowLayout{
                Text {
                    id: filesize
                    font.pixelSize: 12
                    color: "#898989"
                    text: fileSize
                }
                Item{
                    Layout.preferredWidth: 5
                    height: 1
                }

                Text {
                    id: filecondition
                    font.pixelSize: 12
                    color: "#898989"
                    text: type === "sendfile" ? "已发送" : "未接收"
                }
            }
        }
        //图片
        Rectangle{
            Layout.preferredWidth: 40
            Layout.fillHeight: true
            radius: 10
            Image {
                id: image
                width: 40
                height: 40
                source: type === "sendfile" ? "qrc:/image/download.png" : "qrc:/image/to_download.png"
            }
        }
    }

    //点击之后打开文件对话框，选择下载路径
    MouseArea{
        anchors.fill: parent
        onClicked: {

        }
    }


    //下载完成
    function downOk(){

    }

}
