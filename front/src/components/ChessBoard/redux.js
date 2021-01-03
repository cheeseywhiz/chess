import { combineReducers } from 'redux';
import fetch2 from '../../fetch2';
import GameActions from '../Game/redux';

const defaultState = {
    moves: [],
    selected: {
        row: null,
        col: null,
    },
};

const types = {
    moves: {
        set: 'CHESS_BOARD_MOVES_SET',
        clear: 'CHESS_BOARD_MOVES_CLEAR',
    },
    selected: {
        set: 'CHESS_BOARD_SELECTED_SET',
        clear: 'CHESS_BOARD_SELECTED_CLEAR',
    },
};

const actions = {
    moves: {
        set: (moves) => ({
            type: types.moves.set,
            moves,
        }),
        clear: () => ({ type: types.moves.clear }),
    },
    selected: {
        set: (row, col) => ({
            type: types.selected.set,
            row,
            col,
        }),
        clear: () => ({ type: types.selected.clear }),
    },
    getMoves: (gameId, row, col) => (dispatch) => {
        dispatch(actions.moves.clear());
        dispatch(actions.selected.set(row, col));
        fetch2({
            url: '/api/game/moves/',
            params: { gameId, row, col },
        }).then(({ moves }) => {
            dispatch(actions.moves.set(moves));
        }).catch(({ status, message }) => {
            dispatch(actions.selected.clear());
            if (status >= 500) throw new Error(message);
        });
    },
    doMove: (gameId, row, col, row2, col2) => (dispatch) => {
        dispatch(actions.moves.clear());
        dispatch(actions.selected.clear());
        fetch2({
            url: '/api/game/moves/',
            params: {
                gameId, row, col, row2, col2,
            },
            method: 'post',
        }).then((state) => {
            dispatch(GameActions.state.set(state));
        }).catch(({ status, message }) => {
            if (status >= 500) throw new Error(message);
        });
    },
};
export default actions;

export const reducer = combineReducers({
    moves: (state = defaultState.moves, { type, ...action }) => {
        switch (type) {
        case types.moves.set: {
            const { moves } = action;
            return moves; }
        case types.moves.clear:
            return defaultState.moves;
        default:
            return state;
        }
    },

    selected: (state = defaultState.selected, { type, ...action }) => {
        switch (type) {
        case types.selected.set: {
            const { row, col } = action;
            return { row, col }; }
        case types.selected.clear:
            return defaultState.selected;
        default:
            return state;
        }
    },
});
