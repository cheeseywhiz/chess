import { createSelector } from 'reselect';

const loginFormSelector = ({ loginForm }) => loginForm;
const loginFormUsernameSelector = createSelector(
    loginFormSelector,
    ({ username }) => username,
);

const usernameSelector = ({ username }) => username;

const createFormSelector = ({ createForm }) => createForm;
const createFormUsernameSelector = createSelector(
    createFormSelector,
    ({ username }) => username,
);

export default {
    loginForm: {
        username: loginFormUsernameSelector,
    },
    username: usernameSelector,
    createForm: {
        username: createFormUsernameSelector,
    },
};
