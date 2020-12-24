export const types = {
    LOGIN_FORM_UPDATE_USERNAME: 'LOGIN_FORM_UPDATE_USERNAME',
    LOGIN_FORM_CLEAR: 'LOGIN_FORM_CLEAR',
    USERNAME_SET: 'USERNAME_SET',
};

export const defaultLoginForm = {
    username: '',
};

export const loginFormUpdateUsername = (username) => ({
    type: types.LOGIN_FORM_UPDATE_USERNAME,
    username,
});

export const loginFormClear = () => ({
    type: types.LOGIN_FORM_CLEAR,
});

/*
 * username:
 *     uninitialized: null
 *     not logged in: ''
 *     logged in:     non-empty string
 */
export const usernameSet = (username = '') => ({
    type: types.USERNAME_SET,
    username,
});

export const login = (username) => (dispatch) => {
    dispatch(loginFormClear());
    const req = { credentials: 'same-origin' };

    if (username !== undefined) {
        req.method = 'post';
        req.headers = { 'Content-Type': 'application/json' };
        req.body = JSON.stringify({ username });
    }

    fetch('/api/AuthCtrl/login', req).then((response) => {
        if (!response.ok) throw Error(response.statusText);
        return response.json();
    }).then((user) => {
        console.log(user);
        dispatch(usernameSet(username || user.username));
    }).catch((error) => {
        console.log(error);
        dispatch(usernameSet());
    });
};

export const logout = () => (dispatch) => {
    dispatch(usernameSet());
    fetch('/api/AuthCtrl/logout', {
        method: 'post',
        credentials: 'same-origin',
    }).then((response) => {
        if (!response.ok) throw Error(response.statusText);
    }).catch((error) => console.log(error));
};

export const init = () => (dispatch) => dispatch(login());
