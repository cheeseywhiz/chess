import { combineReducers } from 'redux';
import { types } from './actions';

const defaultLoginForm = {
    username: '',
};

const defaultCreateForm = {
    username: '',
};

const defaultNewGame = {
    player: null,
    opponent: '',
};

export default combineReducers({
    loginForm: (state = defaultLoginForm, { type, ...action }) => {
        switch (type) {
        case types.LOGIN_FORM_USERNAME_SET: {
            const { username } = action;
            return { ...state, username }; }
        case types.LOGIN_FORM_CLEAR:
            return defaultLoginForm;
        default:
            return state;
        }
    },

    username: (state = null, { type, ...action }) => {
        switch (type) {
        case types.USERNAME_SET: {
            const { username } = action;
            return username; }
        default:
            return state;
        }
    },

    createForm: (state = defaultCreateForm, { type, ...action }) => {
        switch (type) {
        case types.CREATE_FORM_USERNAME_SET: {
            const { username } = action;
            return { ...state, username }; }
        case types.CREATE_FORM_CLEAR:
            return defaultCreateForm;
        default:
            return state;
        }
    },

    newGame: (state = defaultNewGame, { type, ...action }) => {
        switch (type) {
        case types.NEW_GAME_PLAYER_SET: {
            const { player } = action;
            return { ...state, player }; }
        case types.NEW_GAME_OPPONENT_SET: {
            const { opponent } = action;
            return { ...state, opponent }; }
        case types.NEW_GAME_CLEAR:
            return defaultNewGame;
        default:
            return state;
        }
    },
});
