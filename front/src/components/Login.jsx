import React from 'react';
import { Redirect, withRouter, Link } from 'react-router-dom';
import { connect } from 'react-redux';
import Form from 'react-bootstrap/Form';
import Button from 'react-bootstrap/Button';
import * as actions from '../actions';
import selectors from '../selectors';

const mapStateToProps = (state) => ({
    loginFormUsername: selectors.loginForm.username(state),
    username: selectors.username(state),
});

const mapDispatchToProps = (dispatch) => ({
    updateUsername: (event) => {
        event.preventDefault();
        dispatch(actions.loginFormUsernameSet(event.target.value));
    },
    login: (username) => (event) => {
        event.preventDefault();
        dispatch(actions.login(username));
    },
});

export default withRouter(connect(mapStateToProps, mapDispatchToProps)(
    ({
        loginFormUsername, username, updateUsername, login, location,
    }) => {
        const params = new URLSearchParams(location.search);
        const referrer = params.get('referrer');
        if (username) return <Redirect to={referrer || '/'} />;
        const createUrl = referrer ? `/create?${params}` : '/create';
        return (
            <>
                <Form onSubmit={login(loginFormUsername)}>
                    <Form.Group>
                        <Form.Label>Username</Form.Label>
                        <Form.Control
                            type="text"
                            placeholder="Username"
                            value={loginFormUsername}
                            onChange={updateUsername}
                        />
                    </Form.Group>
                    <Button type="submit">Login</Button>
                </Form>
                <Link to={createUrl}>Create Account</Link>
            </>
        );
    },
));
