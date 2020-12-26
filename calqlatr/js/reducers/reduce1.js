
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
      tempState = deepCopy(state,{});
      tempState[action.id].x += action.x;
      tempState[action.id].y += action.y;

      return tempState;

  case '@mouseReleased':
      //console.log("鼠标按下记录此时的坐标");
      tempState = deepCopy(state,{});
      return tempState;

  case 'INCREMENT':

      console.log("init INCREMENT" + JSON.stringify(state));
      if (root.activeItem !== -1)
      {
          tempState = deepCopy(state,{});
          tempState[root.activeItem].x = state[root.activeItem].x + 10;

          return tempState;
      }
      break;

  case 'DECREMENT':
      console.log("init DECREMENT" + JSON.stringify(state));
      if (root.activeItem !== -1)
      {
          tempState = deepCopy(state,{});
          tempState[root.activeItem].x = state[root.activeItem].x - 10;

          return tempState;
      }
      break;

  default: return state;
  }

  return state;
}
