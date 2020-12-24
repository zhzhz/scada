
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
//深拷贝
function deepCopy(old, newRrray)
{
    var c = newRrray || {};
    for (var i in old)
    {
        if (typeof old[i] === 'object')
        {
            c[i] = (old[i].constructor === Array) ? [] : {};
            deepCopy(old[i], c[i]);
        }
        else
        {
            c[i] = old[i];
        }
    }
    return c;
}

var unImmutableInitState = {
//    item1 : {
//        past:[],
//        present:{x:1,y:2},
//        future:[]
//    },
    item1:  {x:1,y:2},
    item2 : {x:1,y:2}
};


  function reducer1(state = unImmutableInitState.item1, action){
    //var state = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : [];
      console.log("reducer1" + JSON.stringify(state))
    //var action = arguments[1];

    var tempState = null;
    switch (action.type) {
    case 'INCREMENT':

        console.log("init INCREMENT" + JSON.stringify(state));
        tempState = deepCopy(state,{});
        tempState.x = state.x + 10;

        return tempState;

    case 'DECREMENT':
        console.log("init DECREMENT" + JSON.stringify(state));

        tempState = deepCopy(state,{});
        tempState.x = state.x - 10;
        return tempState;

    default: return state;
    }
};

  function reducer2(state = unImmutableInitState.item2, action) {
    //var state = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : [];
    //var action = arguments[1];

    var tempState = null;
    switch (action.type) {
    case '@123':

        console.log("init INCREMENT" + JSON.stringify(state));
        tempState = deepCopy(state,{});
        tempState.x = state.x + 10;

        return tempState;

    case '@456':
        console.log("init DECREMENT" + JSON.stringify(state));

        tempState = deepCopy(state,{});
        tempState.x = state.x - 10;
        return tempState;

    default: return state;
    }
};



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
function updateUI() {

    state = store.getState();
    console.log(JSON.stringify(state) + "updateUI")
    moveItem.x = state.item1.present.x;
    //moveItem.x = state.item2.x;
    //console.log("updateUI：state" + state.index);
    //menuFunSpace_HomeTab_ItemBoard.shapeMode = store.getState().menuFunSpaceFilter.itemBoard.itemBoardSelectIndex
    var b= 3
    var c = 2
    var a = {ss1:b }
 //console.log(formatJson(JSON.stringify(state)) )
    //console.log("render end!")
    //state需要先转换为字符串，再调用formatJson 进行格式化转换输出
    console.log(state.x + "main state")
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
