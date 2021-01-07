
Qt.include("../state.js")
Qt.include("../deepCopy.js")

function reducer1(state = unImmutableInitState.item1, action){
  //var state = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : [];
    console.log("reducer1" + JSON.stringify(state))
  //var action = arguments[1];

  var tempState = null;
  switch (action.type) {
//  case '@mousePressed':
//      console.log("鼠标按下记录此时的坐标");
//      tempState = deepCopy(state,{});
//      return tempState;

  case '@mousePositionChanged':
      console.log("鼠标移动，改变坐标");
      tempState = deepCopy(state,[]);
      tempState[action.id].x += action.x;
      tempState[action.id].y += action.y;

      return tempState;

  case '@mouseReleased':
      console.log("鼠标按下记录此时的坐标");
      tempState = deepCopy(state,[]);
      return tempState;

  case '@createItem':
      console.log("创建新对象" + action.id + ' ' + action.x + " " + action.y);
//      state[action.id] = {};
//      state[action.id].x = action.x;
//      state[action.id].y = action.y;

      tempState = deepCopy(state,[]);
      tempState[action.id] = {};
      tempState[action.id].x = action.x;
      tempState[action.id].y = action.y;
      tempState[action.id].width = action.width;
      tempState[action.id].height = action.height;
      tempState[action.id].rotation = action.rotation;
      console.log( JSON.stringify(tempState));
      return tempState;

  case '@deleteItem' :
      tempState = deepCopy(state,[]);
      tempState[action.index] = undefined;
      return tempState;

  case 'INCREMENT':

      console.log("init INCREMENT" + JSON.stringify(state));
      if (root.activeItemId !== -1)
      {
          tempState = deepCopy(state,{});
          tempState[root.activeItemId].x = state[root.activeItemId].x + 10;

          return tempState;
      }
      break;

  case 'DECREMENT':
      console.log("init DECREMENT" + JSON.stringify(state));
      if (root.activeItem !== -1)
      {
          tempState = deepCopy(state,{});
          tempState[root.activeItemId].x = state[root.activeItemId].x - 10;

          return tempState;
      }
      break;

  default: return state;
  }

  return state;
}
