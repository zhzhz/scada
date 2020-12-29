//.import "../state.js" as UnImmutableInitState
Qt.include("../state.js")
Qt.include("../deepCopy.js")

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
