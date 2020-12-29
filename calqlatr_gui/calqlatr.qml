import QtQuick 2.9
import QtQuick.Controls 2.5
import "js/MainStateControl.js" as MainStateControl

//gui只留下rootCanvas
Rectangle {
    id:rootCanvas
    width: 800
    height: 480
    color: "yellow"
    property var x_init:0
    property var itemID:0
    property var items:[]//创建的obj都放在这
    property var count:0
    property var mainComponent: null
    property var mainStateControl : MainStateControl
    //property var activeItemId: -1
    //property var activeItem:null//当前选中的对象，当前选中对象的id

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
    function createItemAgain(itemID)
    {
        //x_init += 120;
        // 获取state数据，用state数据初始化新item。
        var state = mainStateControl.store.getState();
        var x_again = state.item1.present[itemID].x;
        var y_again = state.item1.present[itemID].y;

        console.log("创建已有的方块" + itemID);
        var obj = mainComponent.createObject(rootCanvas,{"text":x_again, "x":x_again,"y": y_again, "id":itemID});
        items[itemID] = obj;
        obj.deleteThis.connect(rootCanvas.deleteItems);
        //console.log("obj.length" + objs.length);
        //itemID++;
    }

    //创建方块
    function createItem(x_init, y_init) {
        //x_init += 120;
        console.log("创建方块" + itemID);

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
                           y:y_init
                       })


        var obj = mainComponent.createObject(rootCanvas,{"text":x_init, "id":itemID});
        //items[items.length] = obj;
        items[itemID] = obj;





        //console.log("创建方块" + JSON.stringify(mainStateControl.store.getState()));
        obj.deleteThis.connect(rootCanvas.deleteItems);
        //console.log("obj.length" + objs.length);
        itemID++;

    }

    Component.onCompleted:{
        mainStateControl.init();
        console.log("mainStateControl store is created!")

        if (mainComponent == null)
            mainComponent = Qt.createComponent('qrc:/content/MoveItem.qml');

        //根据解析的json字符串创建state
        var configFile = inputFile.readfile("a.txt", "../tests");
        //console.log(jsonFile);
        var config = JSON.parse(configFile);
        for (var i = 0; i < config.key.length; i++)
        {
            createItem(config.key[i].x, config.key[i].y);
        }


    }
}