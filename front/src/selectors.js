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

const newGameSelector = ({ newGame }) => newGame;
const newGamePlayerSelector = createSelector(
    newGameSelector,
    ({ player }) => player,
);
const newGameOpponentSelector = createSelector(
    newGameSelector,
    ({ opponent }) => opponent,
);

export default {
    loginForm: {
        username: loginFormUsernameSelector,
    },
    username: usernameSelector,
    createForm: {
        username: createFormUsernameSelector,
    },
    newGame: {
        player: newGamePlayerSelector,
        opponent: newGameOpponentSelector,
    },
};
