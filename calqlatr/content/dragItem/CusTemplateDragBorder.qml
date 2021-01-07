import QtQuick 2.9
import QtQuick.Controls 2.2
//import ".."
//import "../.."

CusResizeBorder {
    id: cusBorder
    //x: 0
    //y: 0
    //width: parent.width
    //height: parent.height
    property int id_num:0
    readonly property int borderMargin: 6
    readonly property int rotateHandleDistance: 25
    //property var currentBlock
    property var controller:{rotation:0}
    property alias dragEnabled: dragItem.enabled
    property bool rotationEnabled: true

    property color rotateHandleColor: "lightgreen"
    property color color: cusConfig.themeColor
    property color borderColor: cusConfig.controlBorderColor

    signal clicked(real x, real y)
    signal doubleClicked(real x, real y)
    //property var type

    //最外围的大方框，中间透明，边框有颜色
    Rectangle {

        border.color: cusConfig.controlBorderColor
        border.width: 1
        color: "transparent"
        radius: borderMargin
        anchors.fill: parent
        anchors.margins: borderMargin + 1
    }
    //line to rotateHandle and Border连接线
    Rectangle {
        color: rotateHandleColor
        width: 2
        visible: rotationEnabled
        height: rotateHandleDistance
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: -rotateHandleDistance
        }
    }
    //top上面中间
    Rectangle {
        z:parent.z
        border.color: cusConfig.controlBorderColor
        border.width: 1
        color: cusConfig.backgroundColor//透明色
        width: borderMargin * 2
        height: width
        radius: width / 2
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }
    }
    //bottom下面中间
    Rectangle {
        border.color: cusConfig.controlBorderColor
        border.width: 1
        color: cusConfig.backgroundColor
        width: borderMargin * 2
        height: width
        radius: width / 2
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
    }
    //left左边中间
    Rectangle {
        border.color: cusConfig.controlBorderColor
        border.width: 1
        color: cusConfig.backgroundColor
        width: borderMargin * 2
        height: width
        radius: width / 2
        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
        }
    }
    //right右边中间
    Rectangle {
        border.color: cusConfig.controlBorderColor
        border.width: 1
        color: cusConfig.backgroundColor
        width: borderMargin * 2
        height: width
        radius: width / 2
        anchors {
            right: parent.right
            verticalCenter: parent.verticalCenter
        }
    }
    //top left左上角
    Rectangle {
        border.color: cusConfig.controlBorderColor
        border.width: 1
        color: cusConfig.backgroundColor
        width: borderMargin * 2
        height: width
        radius: width / 2
        anchors {
            top: parent.top
            left: parent.left
        }
    }//右上角
    Rectangle {
        border.color: cusConfig.controlBorderColor
        border.width: 1
        color: cusConfig.backgroundColor
        width: borderMargin * 2
        height: width
        radius: width / 2
        anchors {
            top: parent.top
            right: parent.right
        }
    }//左下角
    Rectangle {
        border.color: cusConfig.controlBorderColor
        border.width: 1
        color: cusConfig.backgroundColor
        width: borderMargin * 2
        height: width
        radius: width / 2
        anchors {
            left: parent.left
            bottom: parent.bottom
        }
    }//右下角
    Rectangle {
        border.color: cusConfig.controlBorderColor
        border.width: 1
        color: cusConfig.backgroundColor
        width: borderMargin * 2
        height: width
        radius: width / 2
        anchors {
            right: parent.right
            bottom: parent.bottom
        }
    }
    //旋转按钮
    Rectangle {
        color: rotateHandleColor
        width: borderMargin * 2
        height: width
        radius: width / 2
        visible: rotationEnabled
        anchors {
            top: parent.top//需要设置为当前方块
            horizontalCenter: parent.horizontalCenter
            topMargin: -rotateHandleDistance
        }
        CusImage {
            id: rotateCursor
            //source: cusConfig.imagePathPrefix + "rotate.png"
            //source: "file:///./../Images/" + "rotate.png"
            source : "qrc:/content/Images/rotate.png"
            visible: rotateArea.containsMouse | rotateArea.pressed
            x: rotateArea.mouseX - width / 2
            y: rotateArea.mouseY - height / 2
        }
        MouseArea {
            id: rotateArea
            anchors.centerIn: parent
            width: parent.width * 2
            height: parent.height * 2
            hoverEnabled: true
            property int lastX: 0
            property real rotationOld: 0
            onContainsMouseChanged: {
                if (containsMouse) {
                    cursorShape = Qt.BlankCursor
                } else {
                    cursorShape = Qt.ArrowCursor
                }
            }
            onPressedChanged: {
                if (containsPress) {
                    lastX = mouseX
                }
            }
            onPositionChanged: {
                if (pressed) {
                    var t = controller.rotation + (mouseX - lastX) / 5
                    t = t % 360
                    //controller.rotation = t
                    console.log("--------------------------------更改rotation");
                    mainStateControl.mouseRotation(id_num, t);
                    //手动增加拖拉框布局代码
                    //cusTemplateDragBorder.anchors.centerIn = controller
                }
            }
            onPressed: {
                //鼠标按下时，记录鼠标初始位置
                //lastX = mouseX
                //lastY = mouseY

                //root.activeItemId = id;
                //root.activeItem = parent.parent;
                console.log("鼠标旋转");
                var state = mainStateControl.store.getState();
                rotationOld = state.item1.present[id_num].rotation;
            }

            onReleased:{
                //如果方块位置变化了，则调用这个函数，反之不调用
                var state = mainStateControl.store.getState();
                var rotation = state.item1.present[id_num].rotation;

                if (rotationOld === rotation)
                {
                    //鼠标位置没变化，不在redux-undo中记录
                }
                else
                {

                    mainStateControl.mouseReleased();
                }
            }


        }
        BasicTooltip {
            id: toolTip
            x: rotateArea.mouseX + 30
            y: rotateArea.mouseY
            visible: rotateArea.pressed
            text: parseInt(controller.rotation) + "°"
        }
    }
    MouseArea {//这个地方只需要判断x和y是否改变，手型拖动
        id: dragItem
        anchors.fill: parent
        anchors.margins: borderMargin * 2
        cursorShape: Qt.PointingHandCursor
        property real lastX: 0
        property real lastY: 0
        property real rectangleXOld: 0
        property real rectangleYOld: 0
        onPressed: {
            //鼠标按下时，记录鼠标初始位置
            lastX = mouseX
            lastY = mouseY

            //root.activeItemId = id;
            root.activeItem = parent.parent;//记录当前控件id
            console.log("鼠标按下，记录方块位置");
            var state = mainStateControl.store.getState();
            rectangleXOld = state.item1.present[id_num].x;
            rectangleYOld = state.item1.present[id_num].y;
        }

        onReleased:{
            //如果方块位置变化了，则调用这个函数，反之不调用
            var state = mainStateControl.store.getState();
            var x = state.item1.present[id_num].x;
            var y = state.item1.present[id_num].y;

            if (rectangleXOld === x && rectangleYOld === y)
            {
                //鼠标位置没变化，不在redux-undo中记录
            }
            else
            {

                mainStateControl.mouseReleased();
            }
        }

        onPositionChanged: {
            if (pressed) {
                //鼠标按住的前提下，坐标改变时，计算偏移量，应用到目标item的坐标上即可
                //moveItem.x += mouseX - lastX
                //moveItem.y += mouseY - lastY
                console.log("鼠标移动---------------" + lastX + ' ' + lastY+ ' ' + mouseX+ ' ' + mouseY);
                mainStateControl.mousePositionChanged(id_num,mouseX - lastX, mouseY - lastY)
                //mouseX在onReleased中等于lastX，所以在这记录下mouse的x，y
                //mouseXTrue = mouseX;
                //mouseYTrue = mouseY;
            }
        }
        //drag.target: controller
        onClicked: {
            cusBorder.clicked(x, y)
            //console.log("cusBorder.clicked(x, y)")

        }
        onDoubleClicked: {
            cusBorder.doubleClicked(x, y)
        }
    }
}
