import QtQuick
import Qt5Compat.GraphicalEffects

// 自定义头像控件
// 采用遮罩的方式，使得图像是带有圆角边框的
Rectangle {
    id: container
    radius: imageRadius
    property var headUrl
    property var imageRadius: 5
    //定义一个图像元素
    Image {
        id: _image
        anchors.fill:parent
        fillMode: Image.PreserveAspectFit
        source: headUrl
        visible: false
    }
    //定义一个遮罩
    Rectangle {
        id: _mask
        color: "black"
        anchors.fill: parent
        radius: imageRadius    //圆角
        visible: false
    }
    //OpacityMask用于将遮罩运用到图像之上
    OpacityMask {
        id:mask_image
        anchors.fill: container
        source: _image
        maskSource: _mask        // 将source使用icon的规则在icon上显示
        visible: true
        // antialiasing: true
    }
}
