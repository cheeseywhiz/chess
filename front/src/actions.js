export const types = {
    LOGIN_FORM_USERNAME_SET: 'LOGIN_FORM_USERNAME_SET',
    LOGIN_FORM_CLEAR: 'LOGIN_FORM_CLEAR',
    USERNAME_SET: 'USERNAME_SET',
    CREATE_FORM_USERNAME_SET: 'CREATE_FORM_USERNAME_SET',
    CREATE_FORM_CLEAR: 'CREATE_FORM_CLEAR',
};

export const loginFormUsernameSet = (username) => ({
    type: types.LOGIN_FORM_USERNAME_SET,
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

export const createFormUsernameSet = (username) => ({
    type: types.CREATE_FORM_USERNAME_SET,
    username,
});

export const createFormClear = () => ({
    type: types.CREATE_FORM_CLEAR,
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

export const create = (username) => (dispatch) => {
    dispatch(createFormClear());
    fetch('/api/AuthCtrl/create', {
        method: 'post',
        credentials: 'same-origin',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ username }),
    }).then((response) => {
        if (!response.ok) throw Error(response.statusText);
        return response.json();
    }).then((user) => {
        console.log(user);
        dispatch(login(username));
    }).catch((error) => console.log(error));
};
