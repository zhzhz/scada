import QtQuick 2.9
import QtQuick.Controls 2.2
Item {
    id: root
    property int id_num:cusBorder.id_num
    property alias containsMouse: mouseArea.containsMouse
    signal posChange(int xOffset, int yOffset)
    implicitWidth: 12
    implicitHeight: 12
    property int posType: Qt.ArrowCursor

    //5.10之前, qml是不能定义枚举的，用只读的int属性代替一下。
    readonly property int posLeftTop: Qt.SizeFDiagCursor
    readonly property int posLeft: Qt.SizeHorCursor
    readonly property int posLeftBottom: Qt.SizeBDiagCursor
    readonly property int posTop: Qt.SizeVerCursor
    readonly property int posBottom: Qt.SizeVerCursor
    readonly property int posRightTop: Qt.SizeBDiagCursor
    readonly property int posRight: Qt.SizeHorCursor
    readonly property int posRightBottom: Qt.SizeFDiagCursor
    MouseArea {//这个地方需要同时判断x,y,width,height是否改变
        id: mouseArea
        anchors.fill: parent//这个是谁？
        hoverEnabled: true
        property int lastX: 0
        property int lastY: 0
        property int rectangleXOld: 0
        property int rectangleYOld: 0
        property int rectangleWidthOld: 0
        property int rectangleHeightOld: 0

        onContainsMouseChanged: {
            if (containsMouse) {
                cursorShape = posType;
            } else {
                cursorShape = Qt.ArrowCursor;
            }
        }
        onPressedChanged: {
            if (containsPress) {
                lastX = mouseX;
                lastY = mouseY;


            }
        }
        onPositionChanged: {
            if (pressed) {
                posChange(mouseX - lastX, mouseY - lastY)
            }
        }


        onPressed: {
            var state = mainStateControl.store.getState();
            rectangleXOld = state.item1.present[id_num].x;
            rectangleYOld = state.item1.present[id_num].y;
            rectangleWidthOld = state.item1.present[id_num].width;
            rectangleHeightOld = state.item1.present[id_num].height;
            console.log("----------------------------------1 " + rectangleXOld +' ' +rectangleYOld);
        }

        onReleased:{//鼠标弹起，触发state记录
            var state = mainStateControl.store.getState();
            var x = state.item1.present[id_num].x;
            var y = state.item1.present[id_num].y;
            var width = state.item1.present[id_num].width;
            var height = state.item1.present[id_num].height;
            console.log("----------------------------------2 " + x +' ' +y);
            if (rectangleXOld === x && rectangleYOld === y && rectangleWidthOld === width && rectangleHeightOld === height)
            {
                //鼠标位置没变化，不在redux-undo中记录
            }
            else
            {

                mainStateControl.mouseReleased();
            }
        }
    }
}
