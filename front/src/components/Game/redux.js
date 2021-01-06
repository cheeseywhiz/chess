import { combineReducers } from 'redux';
import HistoryActions from '../History/redux';
import fetch2 from '../../fetch2';

const gameKeys = [
    'white', 'black', 'gameId',
];

const stateKeys = [
    'stateId', 'state', 'player', 'endgameState', 'nMoves', 'whiteCaptures', 'blackCaptures', 'board',
];

const defaultState = (() => {
    const gameState = {};
    gameKeys.forEach((key) => {
        gameState[key] = null;
    });
    gameState.state = {};
    stateKeys.forEach((key) => {
        gameState.state[key] = null;
    });
    return gameState;
})();

const types = {
    state: {
        set: 'GAME_STATE_SET',
    },
    set: 'GAME_SET',
    clear: 'GAME_CLEAR',
};

const actions = {
    state: {
        set: (state) => ({
            type: types.state.set,
            state,
        }),
        stateId: {
            load: (stateId) => (dispatch) => {
                fetch2({
                    url: '/api/state/',
                    params: { stateId },
                }).then((state) => {
                    dispatch(actions.state.set(state));
                }).catch(({ status, message, reason }) => {
                    dispatch(actions.clear());
                    if (status >= 500) throw new Error(message);
                    throw new Error(reason);
                });
            },
        },
    },
    set: (game) => ({
        type: types.set,
        game,
    }),
    clear: () => ({ type: types.clear }),
    init: (gameId) => (dispatch) => {
        fetch2({
            url: '/api/game/',
            params: { gameId },
        }).then((game) => {
            dispatch(actions.set(game));
            const { state } = game;
            const { stateId } = state;
            dispatch(HistoryActions.load(gameId, stateId));
        }).catch(({ status, message, reason }) => {
            dispatch(actions.clear());
            if (status >= 500) throw new Error(message);
            throw new Error(reason);
        });
    },
};
export default actions;

export const reducer = (() => {
    const gameReducers = {};
    gameKeys.forEach((key) => {
        gameReducers[key] = (state = defaultState[key], { type, ...action }) => {
            switch (type) {
            case types.set: {
                const { game } = action;
                return game[key]; }
            case types.clear:
                return defaultState[key];
            default:
                return state;
            }
        };
    });

    const stateReducers = {};
    stateKeys.forEach((key) => {
        stateReducers[key] = (state = defaultState.state[key], { type, ...action }) => {
            switch (type) {
            case types.state.set:
                return action.state[key];
            case types.set: {
                const { game } = action;
                return game.state[key]; }
            case types.clear:
                return defaultState.state[key];
            default:
                return state;
            }
        };
    });

    gameReducers.state = combineReducers(stateReducers);
    return combineReducers(gameReducers);
})();
