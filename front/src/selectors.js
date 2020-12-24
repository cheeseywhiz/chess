import { createSelector } from 'reselect';

const loginFormSelector = ({ loginForm }) => loginForm;
const usernameSelector = ({ username }) => username;

const loginFormUsernameSelector = createSelector(
    loginFormSelector,
    ({ username }) => username,
);

export default {
    loginForm: {
        username: loginFormUsernameSelector,
    },
    username: usernameSelector,
};
