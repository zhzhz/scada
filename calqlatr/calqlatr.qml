import QtQuick 2.9
import QtQuick.Controls 2.5
import "js/MainStateControl.js" as MainStateControl

Rectangle {
    id:root
    width: 1024
    height: 1024
    color: "yellow"
    property var mainComponent: null

    property var mainStateControl : MainStateControl
    property var activeItemId: -1
    property var activeItem:null//当前选中的对象，当前选中对象的id

    Rectangle {
        id:rootCanvas
        x:0
        y:300
        width: 800
        height: 480
        border.color: "black"
        border.width: 1
        color: "transparent"
        property var x_init:0
        property var itemID:0

        function deleteItems(object) {
            object.destroy()
        }

        MouseArea {
            anchors.fill: parent
            onPressed: {
                //mainStateControl.mainUndo()
                //console.log("向后")
            }
        }

        //创建方块
        function createItem() {
            x_init += 120;
            console.log("创建方块" + itemID);

            //新建state的初始状态,要在创建moveiem之前创建
            var state = mainStateControl.store.getState();
            state.item1.present[itemID] = {x:x_init, y:0};

            var obj = mainComponent.createObject(rootCanvas,{"text":x_init, "x":x_init, "id":itemID});

            itemID++;
            console.log("创建方块" + JSON.stringify(mainStateControl.store.getState()));
            obj.deleteThis.connect(rootCanvas.deleteItems);
        }
    }

    Rectangle {
        width: 100
        height: 100

        color: "lightblue"
        Text {
            //id: btnText
            anchors.centerIn: parent
            text: qsTr("x加")
        }

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

        Text {
            //id: btnText
            anchors.centerIn: parent
            text: qsTr("x减")
        }

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

        Text {
            //id: btnText
            anchors.centerIn: parent
            text: qsTr("撤销")
        }

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

        Text {
            //id: btnText
            anchors.centerIn: parent
            text: qsTr("回撤")
        }

        MouseArea {
            anchors.fill: parent
            onPressed: {
                //mainStateControl.mainRedo()
                mainStateControl.redo()

            }
        }
    }

    Rectangle {
        x: 500
        width: 100
        height: 100

        color: "lightblue"

        Text {
            //id: btnText
            anchors.centerIn: parent
            text: qsTr("增加方块")
        }

        MouseArea {
            anchors.fill: parent
            onPressed: {
                //mainStateControl.mainRedo()
               // mainStateControl.redo()
                rootCanvas.createItem();

            }
        }
    }

    Rectangle {
        x: 620
        width: 100
        height: 100

        color: "lightblue"

        Text {
            //id: btnText
            anchors.centerIn: parent
            text: qsTr("删除方块")
        }

        MouseArea {
            anchors.fill: parent
            onPressed: {
                if (activeItem !== null)
                {
                    activeItem.deleteThis(activeItem);
                }
            }
        }
    }



    Component.onCompleted:{
        mainStateControl.init();
        console.log("mainStateControl store is created!")
        //mainStateControl.updateUI();

        if (mainComponent == null)
            mainComponent = Qt.createComponent('qrc:/content/MoveItem.qml');

        //console.log("mainComponent" + mainComponent);

    }
}
