export const types = {
    LOGIN_FORM_USERNAME_SET: 'LOGIN_FORM_USERNAME_SET',
    LOGIN_FORM_USERNAME_INVALID_SET: 'LOGIN_FORM_USERNAME_INVALID_SET',
    LOGIN_FORM_CLEAR: 'LOGIN_FORM_CLEAR',
    USERNAME_SET: 'USERNAME_SET',
    CREATE_FORM_USERNAME_SET: 'CREATE_FORM_USERNAME_SET',
    CREATE_FORM_USERNAME_INVALID_SET: 'CREATE_FORM_USERNAME_INVALID_SET',
    CREATE_FORM_CLEAR: 'CREATE_FORM_CLEAR',
    NEW_GAME_PLAYER_SET: 'NEW_GAME_PLAYER_SET',
    NEW_GAME_PLAYER_INVALID_SET: 'NEW_GAME_PLAYER_INVALID_SET',
    NEW_GAME_OPPONENT_SET: 'NEW_GAME_OPPONENT_SET',
    NEW_GAME_OPPONENT_INVALID_SET: 'NEW_GAME_OPPONENT_INVALID_SET',
    NEW_GAME_CLEAR: 'NEW_GAME_CLEAR',
};

export const defaultState = {
    loginForm: {
        username: {
            text: '',
            invalid: '',
        },
    },

    username: null,

    createForm: {
        username: {
            text: '',
            invalid: '',
        },
    },

    newGame: {
        player: {
            text: '',
            invalid: '',
        },
        opponent: {
            text: '',
            invalid: '',
        },
    },
};

export const loginFormUsernameSet = (username) => ({
    type: types.LOGIN_FORM_USERNAME_SET,
    username,
});

export const loginFormUsernameInvalidSet = (invalid = defaultState.loginForm.username.invalid) => ({
    type: types.LOGIN_FORM_USERNAME_INVALID_SET,
    invalid,
});

export const loginFormClear = () => ({
    type: types.LOGIN_FORM_CLEAR,
});

/*
 * username:
 *     uninitialized: null
 *     not logged in: ''
 *     logged in: non-empty string
 */
export const usernameSet = (username = '') => ({
    type: types.USERNAME_SET,
    username,
});

export const createFormUsernameSet = (username) => ({
    type: types.CREATE_FORM_USERNAME_SET,
    username,
});

export const createFormUsernameInvalidSet = (
    invalid = defaultState.createForm.username.invalid,
) => ({
    type: types.CREATE_FORM_USERNAME_INVALID_SET,
    invalid,
});

export const createFormClear = () => ({
    type: types.CREATE_FORM_CLEAR,
});

export const newGamePlayerSet = (player) => ({
    type: types.NEW_GAME_PLAYER_SET,
    player,
});

export const newGamePlayerInvalidSet = (invalid = defaultState.newGame.player.invalid) => ({
    type: types.NEW_GAME_PLAYER_INVALID_SET,
    invalid,
});

export const newGameOpponentSet = (opponent) => ({
    type: types.NEW_GAME_OPPONENT_SET,
    opponent,
});

export const newGameOpponentInvalidSet = (invalid = defaultState.newGame.opponent.invalid) => ({
    type: types.NEW_GAME_OPPONENT_INVALID_SET,
    invalid,
});

export const newGameClear = () => ({
    type: types.NEW_GAME_CLEAR,
});

// fetch with improved interface
// cookies are included by default
// option json sets Content-Type header and sets default method to POST
// see https://developer.mozilla.org/en-US/docs/Web/API/Response#An_Ajax_Call for ajax inspiration
const fetch2 = async ({ url, json, ...optionsIn }) => {
    let options = { credentials: 'same-origin', headers: {}, ...optionsIn };

    if (json !== undefined) {
        options = { method: 'post', ...options, body: JSON.stringify(json) };
        options.headers['Content-Type'] = 'application/json';
    }

    const response = await fetch(url, options);
    if (response.status >= 500) throw new Error(response.statusText);
    if (response.status === 204) return Promise.resolve();
    const jsonRes = await response.json();

    if (response.status >= 400) {
        console.log(jsonRes);
        return Promise.reject(jsonRes);
    }

    return Promise.resolve(jsonRes);
};

export const login = (username) => (dispatch) => {
    const req = { url: '/api/AuthCtrl/login' };
    if (username) req.json = { username };
    fetch2(req)
        .then((user) => {
            console.log(user);
            dispatch(usernameSet(username || user.username));
            dispatch(loginFormUsernameInvalidSet());
        }).catch(({ reason }) => {
            dispatch(usernameSet());
            dispatch(loginFormUsernameInvalidSet(reason));
        });
};

export const logout = () => (dispatch) => {
    dispatch(usernameSet());
    fetch2({ url: '/api/AuthCtrl/logout', method: 'post' });
};

export const init = () => (dispatch) => dispatch(login());

export const create = (username) => (dispatch) => {
    fetch2({
        url: '/api/AuthCtrl/create',
        json: { username },
    }).then(() => {
        dispatch(login(username));
        dispatch(createFormUsernameInvalidSet());
    }).catch(({ reason }) => {
        dispatch(createFormUsernameInvalidSet(reason));
    });
};

export const submitNewGame = (player, opponent) => (dispatch) => {
    fetch2({
        url: '/api/GamesCtrl/new_game',
        json: { player, opponent },
    }).then((game) => {
        console.log(game);
        dispatch(newGamePlayerInvalidSet());
        dispatch(newGameOpponentInvalidSet());
    }).catch(({ reason }) => {
        dispatch(newGamePlayerInvalidSet(reason.includes('player') ? reason : ''));
        dispatch(newGameOpponentInvalidSet(reason.includes('opponent') ? reason : ''));
    });
};
