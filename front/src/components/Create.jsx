import React from 'react';
import { Redirect, withRouter } from 'react-router-dom';
import { connect } from 'react-redux';
import Form from 'react-bootstrap/Form';
import Button from 'react-bootstrap/Button';
import * as actions from '../actions';
import selectors from '../selectors';

const mapStateToProps = (state) => ({
    username: selectors.username(state),
    createFormUsername: selectors.createForm.username(state),
});

const mapDispatchToProps = (dispatch) => ({
    clear: () => dispatch(actions.createFormClear()),
    updateUsername: (event) => {
        event.preventDefault();
        dispatch(actions.createFormUsernameSet(event.target.value));
    },
    create: (username) => (event) => {
        event.preventDefault();
        dispatch(actions.create(username));
    },
});

class Create extends React.Component {
    componentDidMount() {
        const { clear } = this.props;
        clear();
    }

    render() {
        const {
            username, createFormUsername, updateUsername, create, location,
        } = this.props;
        const params = new URLSearchParams(location.search);
        const referrer = params.get('referrer');
        if (username) return <Redirect to={referrer || '/'} />;
        return (
            <Form onSubmit={create(createFormUsername)}>
                <Form.Group>
                    <Form.Label>Username</Form.Label>
                    <Form.Control
                        type="text"
                        placeholder="Username"
                        value={createFormUsername}
                        onChange={updateUsername}
                    />
                </Form.Group>
                <Button type="submit">Create Account</Button>
            </Form>
        );
    }
}

export default withRouter(connect(mapStateToProps, mapDispatchToProps)(Create));
