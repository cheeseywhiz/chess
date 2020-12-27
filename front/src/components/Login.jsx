import React from 'react';
import { Redirect, withRouter, Link } from 'react-router-dom';
import { connect } from 'react-redux';
import Form from 'react-bootstrap/Form';
import Button from 'react-bootstrap/Button';
import * as actions from '../actions';

const mapStateToProps = ({ loginForm, username }) => ({
    loginForm, username,
});

const mapDispatchToProps = (dispatch) => ({
    clear: () => dispatch(actions.loginFormClear()),
    updateUsername: (event) => {
        event.preventDefault();
        dispatch(actions.loginFormUsernameSet(event.target.value));
    },
    login: (username) => (event) => {
        event.preventDefault();
        dispatch(actions.login(username));
    },
});

class Login extends React.Component {
    componentDidMount() {
        const { clear } = this.props;
        clear();
    }

    render() {
        const {
            loginForm, username, updateUsername, login, location,
        } = this.props;
        const params = new URLSearchParams(location.search);
        const referrer = params.get('referrer');
        if (username) return <Redirect to={referrer || '/'} />;
        const createUrl = referrer ? `/create?${params}` : '/create';
        return (
            <>
                <Form noValidate onSubmit={login(loginForm.username.text)}>
                    <Form.Group>
                        <Form.Label>Username</Form.Label>
                        <Form.Control
                            type="text"
                            placeholder="Username"
                            value={loginForm.username.text}
                            onChange={updateUsername}
                            isInvalid={loginForm.username.invalid}
                        />
                        <Form.Control.Feedback type="invalid">
                            {loginForm.username.invalid}
                        </Form.Control.Feedback>
                    </Form.Group>
                    <Button type="submit">Login</Button>
                </Form>
                <Link to={createUrl}>Create Account</Link>
            </>
        );
    }
}

export default withRouter(connect(mapStateToProps, mapDispatchToProps)(Login));
