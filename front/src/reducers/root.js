import { combineReducers } from 'redux';
import * as actions from '../actions';

export default combineReducers({
    loginForm: (state = actions.defaultLoginForm, { type, ...action }) => {
        switch (type) {
        case actions.types.LOGIN_FORM_UPDATE_USERNAME: {
            const { username } = action;
            return { ...state, username }; }
        case actions.types.LOGIN_FORM_CLEAR:
            return actions.defaultLoginForm;
        default:
            return state;
        }
    },

    username: (state = null, { type, ...action }) => {
        switch (type) {
        case actions.types.USERNAME_SET: {
            const { username } = action;
            return username; }
        default:
            return state;
        }
    },
});
