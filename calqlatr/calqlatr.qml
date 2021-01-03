import QtQuick 2.9
import QtQuick.Controls 2.5
import "js/MainStateControl.js" as MainStateControl

Rectangle {
    id:root
    width: 1024
    height: 1024
    color: "yellow"
    property var mainComponent: null
    property var digitComponent: null
    property var type:(new Map())

    property var mainStateControl : MainStateControl
    //property var activeItemId: -1
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
        property var items:[]//创建的obj都放在这
        property var deleteNowItems:[]
        property var count:0

        function deleteItems(object) {
            console.log("删除对象" + object);
            object.destroy();
        }

        MouseArea {
            anchors.fill: parent
            onPressed: {
                //mainStateControl.mainUndo()
                //console.log("向后")
            }
        }

        //创建新item，不增加state，因为state已经从撤销数据中获得了
        //使用原来的id号码
        function createItemAgain(Component, itemID, itemNum)
        {
            //x_init += 120;
            // 获取state数据，用state数据初始化新item。
            var state = mainStateControl.store.getState();
            var x_again = state.item1.present[itemID].x;
            var y_again = state.item1.present[itemID].y;

            console.log("创建已有的方块" + itemID);
            var obj = Component.createObject(rootCanvas,{"text":x_again, "id": itemID, "x":x_again, "y": y_again});
            obj.nameId = itemNum;

            items[itemID] = obj;
            obj.deleteThis.connect(rootCanvas.deleteItems);
            //console.log("obj.length" + objs.length);
            //itemID++;
        }

        //创建方块
        function createItem(Component, itemNum) {
            x_init += 120;
            console.log("创建方块" + itemNum);

            //新建state的初始状态,要在创建moveiem之前创建
            //var state = mainStateControl.store.getState();
            //state.item1.present[itemID] = {x:x_init, y:0};
            ////////////////////////////////////////////////////////
            //修改为action修改state
            //创建方块时不能记录此时的state
            mainStateControl.store.dispatch({
                               type:"@createItem",
                               id:itemID,
                               x:x_init,
                               y:0
                           })


            var obj = Component.createObject(rootCanvas,{"text":x_init, id: itemID});
            obj.nameId = itemNum;

            parent.type.set(obj.objectName, Component);//记录当前Component的类型

            //items[items.length] = obj;
            items[itemID] = obj;





            //console.log("创建方块" + JSON.stringify(mainStateControl.store.getState()));
            obj.deleteThis.connect(rootCanvas.deleteItems);
            //console.log("obj.length" + objs.length);
            itemID++;

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
        property var i:1

        Text {
            //id: btnText
            anchors.centerIn: parent
            text: qsTr("增加按钮")
        }

        MouseArea {
            anchors.fill: parent
            onPressed: {
                //mainStateControl.mainRedo()
               // mainStateControl.redo()
                rootCanvas.createItem(mainComponent,  parent.i);
                parent.i++;
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
                    //修改state，自动触发删除items
//                    for (var i = 0; i < rootCanvas.items.length; i++)
//                    {
//                        if (rootCanvas.items[i] === activeItem)
//                        {
//                            rootCanvas.items[i] = null;
//                            break;
//                        }
//                    }
//                    activeItem.deleteThis(activeItem);
                    mainStateControl.deleteItem(activeItem);
                    activeItem = null;
                }
            }
        }
    }

    Rectangle {
        x: 740
        width: 100
        height: 100

        color: "lightblue"

        Text {
            //id: btnText
            anchors.centerIn: parent
            text: qsTr("生成json文件")
        }

        MouseArea {
            anchors.fill: parent
            onPressed: {
                //mainStateControl.mainRedo()
               // mainStateControl.redo()
                mainStateControl.createJson();

            }
        }
    }

    Rectangle {
        x: 860
        width: 100
        height: 100
        property var i:1
        color: "lightblue"

        Text {
            //id: btnText
            anchors.centerIn: parent
            text: qsTr("增加数字方块")
        }

        MouseArea {
            anchors.fill: parent
            onPressed: {
                rootCanvas.createItem(digitComponent,  parent.i);
                parent.i++;
            }
        }
    }



    Component.onCompleted:{
        mainStateControl.init();
        console.log("mainStateControl store is created!")
        //mainStateControl.updateUI();//负责管理删除的item

        if (mainComponent == null)
            mainComponent = Qt.createComponent('qrc:/content/MoveItem.qml');
        //console.log("记录mainComponent" + mainComponent);

        if (digitComponent == null)
            digitComponent = Qt.createComponent('qrc:/content/DigitItem.qml');

        //console.log("mainComponent" + mainComponent);

    }
}
