
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
var state;
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
    state = store.getState();
    console.log("init" + JSON.stringify(state));
    //console.log("init" + state.past[0].x);
    //    var copyState = Object.assign({},state)
    //console.log(JSON.parse(formatJson(JSON.stringify(state))).index1);
    //console.log(state.index);
}


//更新画布外界面
//state改变最先调用这，负责删除和创建item
function updateUI() {

    state = store.getState();
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
            //for(var k = 0; k < items_length; k++)
            //   console.log(rootCanvas.items[k]);

            //rootCanvas.objs[i].deleteRender();
            rootCanvas.items[i].deleteThis(rootCanvas.items[i]);
            //rootCanvas.items.pop(rootCanvas.items[i]);//从objs记录中弹出删除的item
            rootCanvas.items[i] = null;
        }

        if (state.item1.present[i] !== undefined && rootCanvas.items[i] === null)
        {
            console.log("创建item");
            rootCanvas.createItemAgain(i);
        }
        //activeItem.deleteThis(activeItem)
    }



}


function mainUndo(){
    console.log("加")
    state = store.getState();
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

    state = store.getState();
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
