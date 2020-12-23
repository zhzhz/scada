.import "helpers.js" as Helpers
//Qt.include("undoActionType.js");
Qt.include("../ObjectAssign.js");

if(typeof UndoActionTypes == "undefined"){
var UndoActionTypes  = {
  UNDO: '@@redux-undo/UNDO',
  REDO: '@@redux-undo/REDO',
  JUMP_TO_FUTURE: '@@redux-undo/JUMP_TO_FUTURE',
  JUMP_TO_PAST: '@@redux-undo/JUMP_TO_PAST',
  JUMP: '@@redux-undo/JUMP',
  CLEAR_HISTORY: '@@redux-undo/CLEAR_HISTORY'
}
};

function _interopRequireWildcard(obj) { if (obj && obj.__esModule) { return obj; } else { var newObj = {}; if (obj != null) { for (var key in obj) { if (Object.prototype.hasOwnProperty.call(obj, key)) newObj[key] = obj[key]; } } newObj.default = obj; return newObj; } }

function _toConsumableArray(arr) { if (Array.isArray(arr)) { for (var i = 0, arr2 = new Array(arr.length); i < arr.length; i++) { arr2[i] = arr[i]; } return arr2; } else { return Array.from(arr); } }

// lengthWithoutFuture: get length of history
function lengthWithoutFuture(history) {
  return history.past.length + 1;
}

// insert: insert `state` into history, which means adding the current state
//         into `past`, setting the new `state` as `present` and erasing
//         the `future`.
function insert(history, state, limit) {
  //debug.log('inserting', state);
 // debug.log('new free: ', limit - lengthWithoutFuture(history));
    //console.log("insert state  1")
  var past = history.past,
      _latestUnfiltered = history._latestUnfiltered;

  var historyOverflow = limit && lengthWithoutFuture(history) >= limit;

  var pastSliced = past.slice(historyOverflow ? 1 : 0);
  var newPast = _latestUnfiltered != null ? [].concat(_toConsumableArray(pastSliced), [_latestUnfiltered]) : pastSliced;

  return {
    past: newPast,
    present: state,
    _latestUnfiltered: state,
    future: []
  };
}

// undo: go back to the previous point in history
function undo(history) {
  var past=  history.past,
      future = history.future,
      _latestUnfiltered = history._latestUnfiltered;

//console.log("do undo! step 2")
  if (past.length <= 0) return history;

  var newFuture = _latestUnfiltered != null ? [_latestUnfiltered].concat(_toConsumableArray(future)) : future;

  var newPresent = past[past.length - 1];
  return {
    past: past.slice(0, past.length - 1), // remove last element from past
    present: newPresent, // set element as new present
    _latestUnfiltered: newPresent,
    future: newFuture
  };
}

// redo: go to the next point in history
function redo(history) {
  var past = history.past,
      future = history.future,
      _latestUnfiltered = history._latestUnfiltered;


  if (future.length <= 0) return history;

  var newPast = _latestUnfiltered != null ? [].concat(_toConsumableArray(past), [_latestUnfiltered]) : past;

  var newPresent = future[0];
  return {
    future: future.slice(1, future.length), // remove element from future
    present: newPresent, // set element as new present
    _latestUnfiltered: newPresent,
    past: newPast
  };
}

// jumpToFuture: jump to requested index in future history
function jumpToFuture(history, index) {
  if (index === 0) return redo(history);
  if (index < 0 || index >= history.future.length) return history;

  var past = history.past,
      future = history.future,
      _latestUnfiltered = history._latestUnfiltered;


  var newPresent = future[index];

  return {
    future: future.slice(index + 1),
    present: newPresent,
    _latestUnfiltered: newPresent,
    past: past.concat([_latestUnfiltered]).concat(future.slice(0, index))
  };
}

// jumpToPast: jump to requested index in past history
function jumpToPast(history, index) {
  if (index === history.past.length - 1) return undo(history);
  if (index < 0 || index >= history.past.length) return history;

  var past = history.past,
      future = history.future,
      _latestUnfiltered = history._latestUnfiltered;


  var newPresent = past[index];

  return {
    future: past.slice(index + 1).concat([_latestUnfiltered]).concat(future),
    present: newPresent,
    _latestUnfiltered: newPresent,
    past: past.slice(0, index)
  };
}

// jump: jump n steps in the past or forward
function jump(history, n) {
  if (n > 0) return jumpToFuture(history, n - 1);
  if (n < 0) return jumpToPast(history, history.past.length + n);
  return history;
}

// createHistory
function createHistory(state, ignoreInitialState) {
  // ignoreInitialState essentially prevents the user from undoing to the
  // beginning, in the case that the undoable reducer handles initialization
  // in a way that can't be redone simply
  return ignoreInitialState ? {
    past: [],
    present: state,
    future: []
  } : {
    past: [],
    present: state,
    _latestUnfiltered: state,
    future: []
  };
}

// helper to dynamically match in the reducer's switch-case
function actionTypeAmongClearHistoryType(actionType, clearHistoryType) {
  return clearHistoryType.indexOf(actionType) > -1 ? actionType : !actionType;
}

// redux-undo higher order reducer
function undoable(reducer) {
  var rawConfig = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : {};

  //debug.set(rawConfig.debug);

  var config = {
    initTypes: (0, Helpers.parseActions)(rawConfig.initTypes, ['@@redux-undo/INIT']),
    limit: rawConfig.limit,
    filter: rawConfig.filter || function () {
      return true;
    },
    undoType: rawConfig.undoType || UndoActionTypes.UNDO,
    redoType: rawConfig.redoType || UndoActionTypes.REDO,
    jumpToPastType: rawConfig.jumpToPastType || UndoActionTypes.JUMP_TO_PAST,
    jumpToFutureType: rawConfig.jumpToFutureType || UndoActionTypes.JUMP_TO_FUTURE,
    jumpType: rawConfig.jumpType || UndoActionTypes.JUMP,
    clearHistoryType: Array.isArray(rawConfig.clearHistoryType) ? rawConfig.clearHistoryType : [rawConfig.clearHistoryType || UndoActionTypes.CLEAR_HISTORY],
    neverSkipReducer: rawConfig.neverSkipReducer || false,
    ignoreInitialState: rawConfig.ignoreInitialState || false
  };

  return function () {
    for (var _len = arguments.length, slices = new Array(_len > 2 ? _len - 2 : 0), _key = 2; _key < _len; _key++) {
      slices[_key - 2] = arguments[_key];
    }

    var state = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : config.history;
    var action = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : {};

    //debug.start(action, state);

    var history = state;


    //console.log("the action is " + action.type)
    if (!config.history) {
      //debug.log('history is uninitialized');

      if (state === undefined) {
        history = config.history = createHistory.apply(undefined, [reducer(state, { type: '@@redux-undo/CREATE_HISTORY' }), config.ignoreInitialState].concat(slices));
        //debug.log('do not initialize on probe actions');
      } else  {
          history = config.history = createHistory(state);
          //debug.log('initialHistory initialized: initialState is not a history', config.history);
      }
    }

    function skipReducer(res) {
      return config.neverSkipReducerva ? res.concat({ present: reducer(res.present.concat(action).concat(slices)) }) : res;
    };

    var res = void 0;
    switch (action.type) {
      case undefined:
        return history;

      case config.undoType:
        res = undo(history);
        //debug.log('perform undo');
        //debug.end(res);
          console.log("undo works");
        return skipReducer(res);

      case config.redoType:
        res = redo(history);
        //debug.log('perform redo');
        ///debug.end(res);
        return skipReducer(res);

      case config.jumpToPastType:
        res = jumpToPast(history, action.index);
        //debug.log('perform jumpToPast to ' + action.index);
        //debug.end(res);
        return skipReducer(res);

      case config.jumpToFutureType:
        res = jumpToFuture(history, action.index);
       // debug.log('perform jumpToFuture to ' + action.index);
        //debug.end(res);
        return skipReducer(res);

      case config.jumpType:
        res = jump(history, action.index);
        //debug.log('perform jump to ' + action.index);
        //debug.end(res);
        return skipReducer(res);

      case actionTypeAmongClearHistoryType(action.type, config.clearHistoryType):
        res = createHistory(history.present);
        //debug.log('perform clearHistory');
       // debug.end(res);
        return skipReducer(res);

      default:
           // console.log("filter is working + 1!")
        res = reducer(state && state.present, action);

        if (config.initTypes.some(function (actionType) {
          return actionType === action.type;
        })) {
           // console.log("filter is working + 2!")
          //debug.log('reset history due to init action');
         // debug.end(config.history);
          return config.history;
        }

        if (history.present === res) {
          // Don't handle this action. Do not call debug.end here,
          // because this action should not produce side effects to the console
           // console.log("filter is working + 3!")
          return history;
        }
       // console.log(action.type)
//console.log(  "config.filter return is " + config.filter(action, res, history) )
        if (typeof config.filter == 'function' && !config.filter(action, res, history)) {
          // if filtering an action, merely update the present
           // console.log("filter is working! + 3333")
          var nextState = Object.assign({},history,{present: res});
           // return history
          //debug.log('filter prevented action, not storing it');
         // debug.end(nextState);
          return nextState;
        } else {
          // If the action wasn't filtered, insert normally
          history = insert(history, res, config.limit);
       // console.log("filter is working! + 4444")
         // debug.log('inserted new state into history');
         // debug.end(history);
          return history;
        }
    }
  };
}

// distinctState helper
function distinctState() {

  return function (action, currentState, previousState) {
      //console.log("filter is working! and the actiontype is  " + action.type);
    //return currentState !== previousState;
    if( action.type !== '@bulidSpace_positionChangeOnCreateItem' && action.type !=="@bulidSpace_createItem" && action.type !== "@bulidSpace_ItemResize" &&  action.type !== "@bulidSpace_ItemRotation")
            {
                return true
            }
    else {

        return false
    }
  };
}
