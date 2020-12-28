import { combineReducers } from 'redux';
import fetch2 from '../../fetch2';
import LoginFormActions from '../LoginForm/redux';

const types = {
    username: {
        set: 'CREATE_FORM_USERNAME_SET',
        invalid: {
            set: 'CREATE_FORM_USERNAME_INVALID_SET',
        },
    },
    clear: 'CREATE_FORM_CLEAR',
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
    submit: (username) => (dispatch) => {
        fetch2({
            url: '/api/AuthCtrl/create',
            json: { username },
        }).then(() => {
            dispatch(LoginFormActions.login(username));
            dispatch(actions.username.invalid.set());
        }).catch(({ reason }) => {
            dispatch(actions.username.invalid.set(reason));
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
