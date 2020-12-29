import { combineReducers } from 'redux';
import fetch2 from '../../fetch2';

const types = {
    player: {
        set: 'NEW_GAME_FORM_PLAYER_SET',
        invalid: {
            set: 'NEW_GAME_FORM_PLAYER_INVALID_SET',
        },
    },
    opponent: {
        set: 'NEW_GAME_FORM_OPPONENT_SET',
        invalid: {
            set: 'NEW_GAME_FORM_OPPONENT_INVALID_SET',
        },
    },
    redirectUrl: {
        set: 'NEW_GAME_FORM_REDIRECT_URL_SET',
    },
    clear: 'NEW_GAME_FORM_CLEAR',
};

const defaultState = {
    player: {
        value: '',
        invalid: '',
    },
    opponent: {
        value: '',
        invalid: '',
    },
    redirectUrl: null,
};

const actions = {
    player: {
        set: (player) => ({
            type: types.player.set,
            player,
        }),
        invalid: {
            set: (invalid = defaultState.player.invalid) => ({
                type: types.player.invalid.set,
                invalid,
            }),
        },
    },
    opponent: {
        set: (opponent) => ({
            type: types.opponent.set,
            opponent,
        }),
        invalid: {
            set: (invalid = defaultState.opponent.invalid) => ({
                type: types.opponent.invalid.set,
                invalid,
            }),
        },
    },
    redirectUrl: {
        set: (redirectUrl = defaultState.redirectUrl) => ({
            type: types.redirectUrl.set,
            redirectUrl,
        }),
    },
    clear: () => ({ type: types.clear }),
    submit: (player, opponent) => (dispatch) => {
        fetch2({
            url: '/api/game/new_game',
            json: { player, opponent },
        }).then(({ gameId }) => {
            dispatch(actions.redirectUrl.set(`/game?gameId=${gameId}`));
            dispatch(actions.player.invalid.set());
            dispatch(actions.opponent.invalid.set());
        }).catch(({ status, message, reason }) => {
            if (status >= 500) throw new Error(message);
            dispatch(actions.player.invalid.set(
                reason.includes('player') ? reason : undefined,
            ));
            dispatch(actions.opponent.invalid.set(
                reason.includes('opponent') ? reason : undefined,
            ));
        });
    },
};
export default actions;

export const reducer = combineReducers({
    player: combineReducers({
        value: (state = defaultState.player.value, { type, ...action }) => {
            switch (type) {
            case types.player.set: {
                const { player } = action;
                return player; }
            case types.clear:
                return defaultState.player.value;
            default:
                return state;
            }
        },

        invalid: (state = defaultState.player.invalid, { type, ...action }) => {
            switch (type) {
            case types.player.invalid.set: {
                const { invalid } = action;
                return invalid; }
            case types.clear:
                return defaultState.player.invalid;
            default:
                return state;
            }
        },
    }),

    opponent: combineReducers({
        value: (state = defaultState.opponent.value, { type, ...action }) => {
            switch (type) {
            case types.opponent.set: {
                const { opponent } = action;
                return opponent; }
            case types.clear:
                return defaultState.opponent.value;
            default:
                return state;
            }
        },

        invalid: (state = defaultState.opponent.invalid, { type, ...action }) => {
            switch (type) {
            case types.opponent.invalid.set: {
                const { invalid } = action;
                return invalid; }
            case types.clear:
                return defaultState.opponent.invalid;
            default:
                return state;
            }
        },
    }),

    redirectUrl: (state = defaultState.redirectUrl, { type, ...action }) => {
        switch (type) {
        case types.redirectUrl.set: {
            const { redirectUrl } = action;
            return redirectUrl; }
        case types.clear:
            return defaultState.redirectUrl;
        default:
            return state;
        }
    },
});
