/* username:
 *     uninitialized: null
 *     not logged in: ''
 *     logged in: non-empty string */
import fetch2 from '../../fetch2';

const types = {
    set: 'USERNAME_SET',
};

const defaultState = null;

const actions = {
    set: (username = '') => ({
        type: types.set,
        username,
    }),
    logout: () => (dispatch) => {
        dispatch(actions.set());
        fetch2({ url: '/api/AuthCtrl/logout', method: 'post' });
    },
};
export default actions;

export const reducer = (state = defaultState, { type, ...action }) => {
    switch (type) {
    case types.set: {
        const { username } = action;
        return username; }
    default:
        return state;
    }
};
