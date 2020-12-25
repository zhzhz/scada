import QtQuick 2.9
import QtQuick.Controls 2.5
import "js/MainStateControl.js" as MainStateControl

Item {
    width: 800
    height: 600
    property var mainStateControl : MainStateControl
    Rectangle {
        width: 100
        height: 100

        color: "lightblue"

        MouseArea {
            anchors.fill: parent
            onPressed: {
                mainStateControl.mainUndo()
                //console.log("向后")
            }
        }
    }

    Rectangle {
        x: 120
        width: 100
        height: 100

        color: "lightblue"

        MouseArea {
            anchors.fill: parent
            onPressed: {
                mainStateControl.mainRedo()
                //console.log("向前")
            }
        }
    }

    Rectangle {
        x: 260
        width: 100
        height: 100

        color: "lightblue"

        MouseArea {
            anchors.fill: parent
            onPressed: {
                //mainStateControl.mainRedo()
                mainStateControl.undo()

            }
        }
    }

    Rectangle {
        x: 380
        width: 100
        height: 100

        color: "lightblue"

        MouseArea {
            anchors.fill: parent
            onPressed: {
                //mainStateControl.mainRedo()
                mainStateControl.redo()

            }
        }
    }

    Rectangle {
        id: moveItem

        //注意拖动目标不要使用锚布局或者Layout，而是使用相对坐标
        x: 100
        y: 100
        width: 300
        height: 200

        color: "lightblue"
        MouseArea {
            anchors.fill: parent
            property real lastX: 0
            property real lastY: 0
            onPressed: {
                //鼠标按下时，记录鼠标初始位置
                lastX = mouseX
                lastY = mouseY
                mainStateControl.mousePressed();
            }
            onReleased:{
                //console.log("鼠标弹起");
                mainStateControl.mouseReleased();
            }
            onPositionChanged: {
                if (pressed) {
                    //鼠标按住的前提下，坐标改变时，计算偏移量，应用到目标item的坐标上即可
                    //moveItem.x += mouseX - lastX
                    //moveItem.y += mouseY - lastY

                    mainStateControl.mousePositionChanged(mouseX - lastX, mouseY - lastY)
                }
            }
        }
    }

    Component.onCompleted:{
        mainStateControl.init();
        console.log("mainStateControl store is created!")
        mainStateControl.updateUI();

    }
}
