import QtQuick 2.9
import QtQuick.Controls 2.2

Item {
    id: root
    //controller 要控制大小的目标，可以是Item，也可以是view，只要提供x、y、width、height等属性的修改
    //默认值为parent
    property var control: parent//指空心的方框，控制的就是这个方框的x，y和width、height
    property int borderWidth: 12

    //左上角的拖拽
    CusDragItem {
        id: leftTopHandle
        posType: posLeftTop
        width: borderWidth
        height: borderWidth//x，y = 0
        onPosChange: {
            //不要简化这个判断条件，化简之后不容易看懂. Qml引擎会自动简化
            if (control.x + xOffset < control.x + control.width)
                //control.x += xOffset;
                mainStateControl.mouseResized(id, xOffset, null, null, null);
            if (control.y + yOffset < control.y + control.height)
                //control.y += yOffset;
                mainStateControl.mouseResized(id, null, yOffset, null, null);
            if (control.width - xOffset > 0)
                //control.width-= xOffset;
                mainStateControl.mouseResized(id, null, null, -xOffset, null);
            if (control.height -yOffset > 0)
                //control.height -= yOffset;
                mainStateControl.mouseResized(id, null, null, null, -yOffset);
        }
    }
    //右上角拖拽
    CusDragItem {
        id: rightTopHandle
        posType: posRightTop
        x: parent.width - width
        width: borderWidth
        height: borderWidth
        onPosChange: {
            //向左拖动时，xOffset为负数
            if (control.width + xOffset > 0)
                //control.width += xOffset;
                mainStateControl.mouseResized(id, null, null, xOffset, null);
            if (control.height - yOffset > 0)
                //control.height -= yOffset;
                mainStateControl.mouseResized(id, null, null, null, -yOffset);
            if (control.y + yOffset < control.y + control.height)
                //control.y += yOffset;
                mainStateControl.mouseResized(id, null, yOffset, null, null);
        }
    }
    //左下角拖拽
    CusDragItem {
        id: leftBottomHandle
        posType: posLeftBottom
        y: parent.height - height
        width: borderWidth
        height: borderWidth
        onPosChange: {
            if (control.x + xOffset < control.x + control.width)
                //control.x += xOffset;
                mainStateControl.mouseResized(id, xOffset, null, null, null);
            if (control.width - xOffset > 0)
                //control.width-= xOffset;
                mainStateControl.mouseResized(id, null, null, -xOffset, null);
            if (control.height + yOffset > 0)
                //control.height += yOffset;
                mainStateControl.mouseResized(id, null, null, null, yOffset);
        }
    }
    //右下角拖拽
    CusDragItem {
        id: rightBottomHandle
        posType: posRightBottom
        x: parent.width - width
        y: parent.height - height
        width: borderWidth
        height: borderWidth
        onPosChange: {
            if (control.width + xOffset > 0)
                //control.width += xOffset;
                mainStateControl.mouseResized(id, null, null, xOffset, null);
            if (control.height + yOffset > 0)
                //control.height += yOffset;
                mainStateControl.mouseResized(id, null, null, null, yOffset);
        }
    }
    //上边拖拽
    CusDragItem {
        posType: posTop
        width: parent.width - leftTopHandle.width - rightTopHandle.width
        height: borderWidth
        x: leftTopHandle.width
        onPosChange: {
            if (control.y + yOffset < control.y + control.height)
               // control.y += yOffset;
                mainStateControl.mouseResized(id, null, yOffset, null, null);
            if (control.height - yOffset > 0)
                //control.height -= yOffset;
                mainStateControl.mouseResized(id, null, null, null, -yOffset);
        }
    }

    //左边拖拽
    CusDragItem {
        posType: posLeft
        height: parent.height - leftTopHandle.height - leftBottomHandle.height
        width: borderWidth

        y: leftTopHandle.height
        onPosChange: {
            if (control.x + xOffset < control.x + control.width)
                //control.x += xOffset;
                mainStateControl.mouseResized(id, xOffset, null, null, null);
            if (control.width - xOffset > 0)
                //control.width-= xOffset;
                mainStateControl.mouseResized(id, null, null, -xOffset, null);
        }
    }
    //右边拖拽
    CusDragItem {
        posType: posRight
        x: parent.width - width
        height: parent.height - rightTopHandle.height - rightBottomHandle.height
        width: borderWidth

        y: rightTopHandle.height
        onPosChange: {
            if (control.width + xOffset > 0)
                //control.width += xOffset;
                mainStateControl.mouseResized(id, null, null, xOffset, null);
        }
    }
    //下边拖拽
    CusDragItem {
        posType: posBottom
        x: leftBottomHandle.width
        y: parent.height - height
        width: parent.width - leftBottomHandle.width - rightBottomHandle.width
        height: borderWidth
        onPosChange: {
            if (control.height + yOffset > 0)
                //control.height += yOffset;
                mainStateControl.mouseResized(id, null, null, null, yOffset);
        }
    }
}
