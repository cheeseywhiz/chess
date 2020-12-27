import { combineReducers } from 'redux';
import { types, defaultState } from './actions';

export default combineReducers({
    loginForm: combineReducers({
        username: combineReducers({
            text: (state = defaultState.loginForm.username.text, { type, ...action }) => {
                switch (type) {
                case types.LOGIN_FORM_USERNAME_SET: {
                    const { username } = action;
                    return username; }
                case types.LOGIN_FORM_CLEAR:
                    return defaultState.loginForm.username.text;
                default:
                    return state;
                }
            },

            invalid: (state = defaultState.loginForm.username.invalid, { type, ...action }) => {
                switch (type) {
                case types.LOGIN_FORM_USERNAME_INVALID_SET: {
                    const { invalid } = action;
                    return invalid; }
                case types.LOGIN_FORM_CLEAR:
                    return defaultState.loginForm.username.invalid;
                default:
                    return state;
                }
            },
        }),
    }),

    username: (state = defaultState.username, { type, ...action }) => {
        switch (type) {
        case types.USERNAME_SET: {
            const { username } = action;
            return username; }
        default:
            return state;
        }
    },

    createForm: combineReducers({
        username: combineReducers({
            text: (state = defaultState.createForm.username.text, { type, ...action }) => {
                switch (type) {
                case types.CREATE_FORM_USERNAME_SET: {
                    const { username } = action;
                    return username; }
                case types.CREATE_FORM_CLEAR:
                    return defaultState.createForm.username.text;
                default:
                    return state;
                }
            },

            invalid: (state = defaultState.createForm.username.invalid, { type, ...action }) => {
                switch (type) {
                case types.CREATE_FORM_USERNAME_INVALID_SET: {
                    const { invalid } = action;
                    return invalid; }
                case types.CREATE_FORM_CLEAR:
                    return defaultState.createForm.username.invalid;
                default:
                    return state;
                }
            },
        }),
    }),

    newGame: combineReducers({
        player: combineReducers({
            text: (state = defaultState.newGame.player.text, { type, ...action }) => {
                switch (type) {
                case types.NEW_GAME_PLAYER_SET: {
                    const { player } = action;
                    return player; }
                case types.NEW_GAME_CLEAR:
                    return defaultState.newGame.player.text;
                default:
                    return state;
                }
            },

            invalid: (state = defaultState.newGame.player.invalid, { type, ...action }) => {
                switch (type) {
                case types.NEW_GAME_PLAYER_INVALID_SET: {
                    const { invalid } = action;
                    return invalid; }
                case types.NEW_GAME_CLEAR:
                    return defaultState.newGame.player.invalid;
                default:
                    return state;
                }
            },
        }),

        opponent: combineReducers({
            text: (state = defaultState.newGame.opponent.text, { type, ...action }) => {
                switch (type) {
                case types.NEW_GAME_OPPONENT_SET: {
                    const { opponent } = action;
                    return opponent; }
                case types.NEW_GAME_CLEAR:
                    return defaultState.newGame.opponent.text;
                default:
                    return state;
                }
            },

            invalid: (state = defaultState.newGame.opponent.invalid, { type, ...action }) => {
                switch (type) {
                case types.NEW_GAME_OPPONENT_INVALID_SET: {
                    const { invalid } = action;
                    return invalid; }
                case types.NEW_GAME_CLEAR:
                    return defaultState.newGame.opponent.invalid;
                default:
                    return state;
                }
            },
        }),
    }),
});
