import { combineReducers } from 'redux';
import fetch2 from '../../fetch2';
import UsernameActions from '../Navbar/UsernameRedux';

const types = {
    username: {
        set: 'LOGIN_FORM_USERNAME_SET',
        invalid: {
            set: 'LOGIN_FORM_USERNAME_INVALID_SET',
        },
    },
    clear: 'LOGIN_FORM_CLEAR',
};

const defaultState = {
    username: {
        value: '',
        invalid: '',
    },
};

const actions = {
    username: {
        set: (username) => ({
            type: types.username.set,
            username,
        }),
        invalid: {
            set: (invalid = defaultState.username.invalid) => ({
                type: types.username.invalid.set,
                invalid,
            }),
        },
    },
    clear: () => ({ type: types.clear }),
    login: (username) => (dispatch) => {
        const req = { url: '/api/AuthCtrl/login' };
        if (username) req.json = { username };
        fetch2(req)
            .then((user) => {
                console.log(user);
                dispatch(UsernameActions.set(username || user.username));
                dispatch(actions.username.invalid.set());
            }).catch(({ status, message, reason }) => {
                if (status >= 500) throw new Error(message);
                dispatch(UsernameActions.set());
                if (username) dispatch(actions.username.invalid.set(reason));
            });
    },
};
export default actions;

export const reducer = combineReducers({
    username: combineReducers({
        value: (state = defaultState.username.value, { type, ...action }) => {
            switch (type) {
            case types.username.set: {
                const { username } = action;
                return username; }
            case types.clear:
                return defaultState.username.value;
            default:
                return state;
            }
        },

        invalid: (state = defaultState.username.invalid, { type, ...action }) => {
            switch (type) {
            case types.username.invalid.set: {
                const { invalid } = action;
                return invalid; }
            case types.clear:
                return defaultState.username.invalid;
            default:
                return state;
            }
        },
    }),
});
