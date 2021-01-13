import { combineReducers } from 'redux';
import fetch2 from '../../fetch2';

const types = {
    games: {
        set: 'USER_GAMES_GAMES_SET',
    },
};

const defaultState = {
    games: [],
};

const actions = {
    games: {
        set: (games) => ({
            type: types.games.set,
            games,
        }),
        load: () => (dispatch) => {
            fetch2({
                url: '/api/user/games',
            }).then((games) => {
                dispatch(actions.games.set(games));
            }).catch(({ status, message, reason }) => {
                if (status >= 500) throw new Error(message);
                throw new Error(reason);
            });
        },
    },
};
export default actions;

export const reducer = combineReducers({
    games: (state = defaultState.games, { type, ...action }) => {
        switch (type) {
        case types.games.set:
            return action.games;
        default:
            return state;
        }
    },
});
