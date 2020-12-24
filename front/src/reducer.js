import { combineReducers } from 'redux';
import { types } from './actions';

const defaultLoginForm = {
    username: '',
};

const defaultCreateForm = {
    username: '',
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
});
