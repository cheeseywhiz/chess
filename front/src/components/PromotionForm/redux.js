import { combineReducers } from 'redux';
import GameActions from '../Game/redux';
import HistoryActions from '../History/redux';
import fetch2 from '../../fetch2';

const types = {
    piece: {
        set: 'PROMOTION_FORM_PIECE_SET',
    },
    clear: 'PROMOTION_FORM_CLEAR',
};

const defaultState = {
    piece: '',
};

const actions = {
    piece: {
        set: (piece) => ({
            type: types.piece.set,
            piece,
        }),
    },
    clear: () => ({ type: types.clear }),
    promote: (gameId, piece) => (dispatch) => {
        fetch2({
            url: '/api/game/moves/promote/',
            params: { gameId, piece },
            method: 'post',
        }).then((state) => {
            dispatch(GameActions.state.set(state));
            const { stateId } = state;
            dispatch(HistoryActions.load(gameId, stateId));
        }).catch(({ status, message, reason }) => {
            if (status >= 500) throw new Error(message);
            throw new Error(reason);
        });
    },
};
export default actions;

export const reducer = combineReducers({
    piece: (state = defaultState.piece, { type, ...action }) => {
        switch (type) {
        case types.piece.set: {
            const { piece } = action;
            return piece; }
        case types.clear:
            return defaultState.piece;
        default:
            return state;
        }
    },
});
