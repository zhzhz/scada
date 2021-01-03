
//引入其他js文件 2种方法都可以
//.import "reducers/index.js" as Reducers
//.import "actions/actions.js" as Actions
//.import "actions/buildSpaceActionTypes.js" as BuildSpaceActionTypes

//immutable函数报错，但是能用
.import "redoUndo/reduxUndo.js" as ReduxUndo
Qt.include("redux.js")

//Qt.include("redux/ObjectAssign.js");
Qt.include("printObj.js");

//Qt.include("immutable.js");
Qt.include("reducers/reduce1.js");
Qt.include("reducers/reduce2.js");


var store;
//var state;
function init(){


    var map1 = {a:1, b:2, c:3};
    delete map1.a;
   // console.log(JSON.stringify(map1) + "删除变量后")


    //state持久化
    // var initState = Immutable.fromJS(unImmutableInitState)

    //store = Redux.createStore(reducer,unImmutableInitState);
    //ReduxUndo.undoable(reducer, {filter:ReduxUndo.distinctState()});
    //ReduxUndo.undoable(reducer);
    //ReduxUndo.undoable(reducer);

    //store = Redux.createStore(ReduxUndo.undoable(reducer, {filter:ReduxUndo.distinctState()}));
    var undoableTodos = ReduxUndo.undoable(reducer1, {filter:ReduxUndo.distinctState()})
    var todoApp = Redux.combineReducers({
//      menuFunSpaceFilter:MenuFunSpaceFilter.menuFunSpaceFilter,
//      todos:HBuildSpaceTodos.undoableTodos,
//      hTabViewFilter:HTabViewFilter.tabViewFilter
        item1:undoableTodos,
        item2:reducer2
    })
    store = Redux.createStore(todoApp);

    store.subscribe(updateUI);
    var state = store.getState();
    console.log("init" + JSON.stringify(state));
    //console.log("init" + state.past[0].x);
    //    var copyState = Object.assign({},state)
    //console.log(JSON.parse(formatJson(JSON.stringify(state))).index1);
    //console.log(state.index);
}


//更新画布外界面
//state改变最先调用这，负责删除和创建item
function updateUI() {

    var state = store.getState();
    console.log(JSON.stringify(state) + "最先调用的updateUI")
    //moveItem.x = state.item1.present.x;
    //moveItem.y = state.item1.present.y;
    //比较objs和state的长度，遍历大的

//    var items_length = 0;//item的长度为包含item个数的长度
//    for (var k = 0; k < rootCanvas.items.length; k++)
//    {
//        if (rootCanvas.items[k] !== null)
//            items_length++;
//    }

    var items_length = rootCanvas.items.length;
    var state_length = state.item1.present.length;
    //console.log("length1" + length1 + ' ' + length2);

    //如果state小于objs，则删除objs
    for (var i = 0; i < items_length; i++)
    {
        console.log("state.item1.present[i] " + i);
        if (state.item1.present[i] === undefined && rootCanvas.items[i] !== null)
        {
            console.log("删除items" + state_length + ' ' + items_length);

            //删除item还要记录item的信息以便重建
            //记录item的类型，是led还是key还是其他，记录item的Num号码
            rootCanvas.deleteNowItems[i] = {};
            rootCanvas.deleteNowItems[i].objectName = rootCanvas.items[i].objectName;
            rootCanvas.deleteNowItems[i].nameId = rootCanvas.items[i].nameId;

            rootCanvas.items[i].deleteRender();
            rootCanvas.items[i].deleteThis(rootCanvas.items[i]);
            //rootCanvas.items.pop(rootCanvas.items[i]);//从objs记录中弹出删除的item
            rootCanvas.items[i] = null;
        }

        if (state.item1.present[i] !== undefined && rootCanvas.items[i] === null)
        {
            console.log("创建item");
            //rootCanvas.createItemAgain(i);
            //根据记录的类型
            //rootCanvas.createItemAgain(Component, i, nameId);
            var obj = rootCanvas.deleteNowItems[i];
            rootCanvas.createItemAgain(root.type.get(obj.objectName), i, obj.nameId);

        }
        //activeItem.deleteThis(activeItem)
    }



}


function mainUndo(){
    console.log("加")
    var state = store.getState();
    //if(state.todos.past.length  > 0){
        store.dispatch({
                           //type:"@@redux-undo/UNDO"
                           type:"INCREMENT"
                           //type:"@123"
                       })
    //}

}

function mainRedo(){

    console.log("减")

    var state = store.getState();
    //if(state.todos.future.length  > 0){
        store.dispatch({
                           type:"DECREMENT"
                           //type:"@456"
                       })
    //}

}

function undo(){
    store.dispatch({
                       type:"@@redux-undo/UNDO"
                   })
}

function redo(){
    store.dispatch({
                       type:"@@redux-undo/REDO"
                   })
}

//function mousePressed(){
//    store.dispatch({
//                       type:"@mousePressed"
//                   })
//}

function mousePositionChanged(id, x, y){
    store.dispatch({
                       type:"@mousePositionChanged",
                       id:id,
                       x:x,
                       y:y
                   })
}

function mouseReleased()
{
    store.dispatch({
                       type:"@mouseReleased"
                   })
}

function deleteItem(deleteItem)
{
    //找到item对应的state id
    var index;
    for (var i = 0; i < rootCanvas.items.length; i++)
    {
        if (rootCanvas.items[i] === deleteItem)
        {
            index = i;
            break;
        }
    }

    store.dispatch({
                       type:"@deleteItem",
                       index:index
                   })

}

//遍历当前state文件,采集x，y。生成json文件

function createJson()
{
    //采集设备采用默认硬编码的
    var jsonFile = {};

    jsonFile.device = [];
    jsonFile.device[0] = {};
    jsonFile.device[0].name = "tcp508neth";
    jsonFile.device[0].id = 1;
    //console.log(JSON.stringify(jsonFile));

    //下面生成key
//    jsonFile.key = [];

//    var state = store.getState();
//    var state_length = state.item1.present.length;
//    var j = 0;
//    for (var i = 0; i < state_length; i++)
//    {
//        if (state.item1.present[i] !== undefined)//state中间有可能是空的
//        {
//            jsonFile.key[j] = {};
//            jsonFile.key[j].name = "key" + (j + 1);

//            jsonFile.key[j].device = "tcp508nserial";

//            jsonFile.key[j].dev_id = 1;

//            jsonFile.key[j].variable = j + 1;

//            //jsonFile.key[j].itemID =

//            //下面输出方块的x和y
//            jsonFile.key[j].x = state.item1.present[i].x;
//            jsonFile.key[j].y = state.item1.present[i].y;

//            j++;
//        }
//    }

    //console.log(JSON.stringify(jsonFile));
    //调用写文件函数，输出配置文件

    ////////////////////////////////////////////////////////////////////////////
    //根据state生成配置
    var state = store.getState();
    var state_length = state.item1.present.length;
    //var j = 0;
    //var init = 0;
    var countId = {};

    for (var i = 0; i < state_length; i++)
    {
        var objs = state.item1.present[i];
        if (objs !== undefined)
        {
            //遍历state中的元素，目前有led有key，生成json字符串
            //state中目前有x，y值，还有的值在rootCanvas.items[i]中
            var objName = rootCanvas.items[i].objectName;
            var objNameId = rootCanvas.items[i].nameId;
            if (jsonFile[objName] === undefined)
            {
                jsonFile[objName] = [];//jsonFile.key = [];jsonFile.led = []
                countId[objName] = 0;
                //init++;
            }
            jsonFile[objName][countId[objName]] = {};
            var config = jsonFile[objName][countId[objName]];

            config.name = objName + objNameId;
            config.device = "tcp508neth";
            config.dev_id = 1;
            config.variable =  countId[objName] + 1;

            config.x = state.item1.present[i].x;
            config.y = state.item1.present[i].y;

            //j++;
            countId[objName]++;
        }
    }

    outputFile.exportToFile(JSON.stringify(jsonFile), "a.txt", "../tests");
}
