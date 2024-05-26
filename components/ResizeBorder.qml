import QtQuick

Rectangle {
    id: root
    color: "transparent"
    // border.width: 4
    // border.color: "black"
    // width: parent.width
    // height: parent.height
    property var control: parent
    //一共8个对象
    //左上角
    ResizeRect {
        id:rec1
        posType: posLeftTop
        onPosChange:(xOffset,yOffset)=> {
            if (control.x + xOffset < control.x + control.width)
                control.x += xOffset;
            if (control.y + yOffset < control.y + control.height)
                control.y += yOffset;
            if (control.width - xOffset > 0)
                control.width-= xOffset;
            if (control.height -yOffset > 0)
                control.height -= yOffset;
        }
    }

    //上面
    ResizeRect {
        id:rec2
        width: root.width - rec1.width * 2
        posType: posTop
        x: rec1.width
        onPosChange:(xOffset,yOffset)=>{
            if (control.y + yOffset < control.y + control.height)
                control.y += yOffset;
            if (control.height - yOffset > 0)
                control.height -= yOffset;
        }
    }

    //右上角
    ResizeRect {
        id:rec3
        posType: posRightTop
        x: parent.width - width
        onPosChange:(xOffset,yOffset)=> {
            //向左拖动时，xOffset为负数
            if (control.width + xOffset > 0)
                control.width += xOffset;
            if (control.height - yOffset > 0)
                control.height -= yOffset;
            if (control.y + yOffset < control.y + control.height)
                control.y += yOffset;
        }
    }

    //左面
    ResizeRect {
        id:rec4
        posType: posLeft
        height: root.height - rec1.height * 2
        y:rec1.height
        onPosChange:(xOffset,yOffset)=> {
            if (control.x + xOffset < control.x + control.width)
                control.x += xOffset;
            if (control.width - xOffset > 0)
                control.width-= xOffset;
        }
    }

    //右面
    ResizeRect {
        id:rec5
        posType: posRight
        height: root.height - rec3.height * 2
        x: parent.width - width
        y: rec3.height
        onPosChange:(xOffset,yOffset)=> {
            if (control.width + xOffset > 0)
                control.width += xOffset;
        }
    }

    //左下角
    ResizeRect {
        id:rec6
        posType: posLeftBottom
        y: parent.height - height
        onPosChange:(xOffset,yOffset)=> {
            if (control.x + xOffset < control.x + control.width)
                control.x += xOffset;
            if (control.width - xOffset > 0)
                control.width-= xOffset;
            if (control.height + yOffset > 0)
                control.height += yOffset;
        }
    }

    //下面
    ResizeRect {
        id:rec7
        posType: posBottom
        x: (parent.width - width) / 2
        y: parent.height - height
        width: parent.width - rec8.width * 2
        onPosChange:(xOffset,yOffset)=> {
            if (control.height + yOffset > 0)
                control.height += yOffset;
        }
    }

    //右下角
    ResizeRect {
        id:rec8
        posType: posRightBottom
        x: parent.width - width
        y: parent.height - height
        onPosChange:(xOffset,yOffset)=> {
            if (control.width + xOffset > 0)
                control.width += xOffset;
            if (control.height + yOffset > 0)
                control.height += yOffset;
        }
    }
}
