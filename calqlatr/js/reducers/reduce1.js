
Qt.include("../state.js")
Qt.include("../deepCopy.js")

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
