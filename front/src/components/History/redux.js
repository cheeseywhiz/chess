import { combineReducers } from 'redux';
import GameActions from '../Game/redux';
import ChessBoardActions from '../ChessBoard/redux';
import fetch2 from '../../fetch2';

const types = {
    set: 'HISTORY_SET',
};

const defaultState = {
    prev: null,
    next: null,
};

const actions = {
    set: (prev, next) => ({
        type: types.set,
        prev,
        next,
    }),
    load: (gameId, stateId) => (dispatch) => {
        dispatch(GameActions.state.stateId.load(stateId));
        dispatch(ChessBoardActions.moves.clear());
        dispatch(ChessBoardActions.selected.clear());
        fetch2({
            url: '/api/history/',
            params: { gameId, stateId },
        }).then(({ prev, next }) => {
            dispatch(actions.set(prev, next));
        }).catch(({ status, message, reason }) => {
            if (status >= 500) throw new Error(message);
            throw new Error(reason);
        });
    },
};
export default actions;

export const reducer = combineReducers({
    prev: (state = defaultState.prev, { type, ...action }) => {
        switch (type) {
        case types.set:
            return action.prev;
        default:
            return state;
        }
    },

    next: (state = defaultState.next, { type, ...action }) => {
        switch (type) {
        case types.set:
            return action.next;
        default:
            return state;
        }
    },
});
