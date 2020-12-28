import React from 'react';
import { Redirect, withRouter } from 'react-router-dom';
import { connect } from 'react-redux';
import Form from 'react-bootstrap/Form';
import Button from 'react-bootstrap/Button';
import CreateFormActions from './redux';

const mapStateToProps = ({ username, createForm }) => ({
    username, createForm,
});

const mapDispatchToProps = (dispatch) => ({
    clear: () => dispatch(CreateFormActions.clear()),
    updateUsername: (event) => {
        event.preventDefault();
        dispatch(CreateFormActions.username.set(event.target.value));
    },
    submit: (username) => (event) => {
        event.preventDefault();
        dispatch(CreateFormActions.submit(username));
    },
});

class CreateForm extends React.Component {
    componentDidMount() {
        const { clear } = this.props;
        clear();
    }

    render() {
        const {
            username, createForm, updateUsername, submit, location,
        } = this.props;
        const params = new URLSearchParams(location.search);
        const referrer = params.get('referrer');
        if (username) return <Redirect to={referrer || '/'} />;
        return (
            <Form onSubmit={submit(createForm.username.value)}>
                <Form.Group>
                    <Form.Label>Username</Form.Label>
                    <Form.Control
                        type="text"
                        placeholder="Username"
                        value={createForm.username.value}
                        onChange={updateUsername}
                        isInvalid={createForm.username.invalid}
                    />
                    <Form.Control.Feedback type="invalid">
                        {createForm.username.invalid}
                    </Form.Control.Feedback>
                </Form.Group>
                <Button type="submit">Create Account</Button>
            </Form>
        );
    }
}

export default withRouter(connect(mapStateToProps, mapDispatchToProps)(CreateForm));
