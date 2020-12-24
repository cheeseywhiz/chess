import React from 'react';
import Navbar from 'react-bootstrap/Navbar';
import Nav from 'react-bootstrap/Nav';
import NavDropdown from 'react-bootstrap/NavDropdown';
import { LinkContainer } from 'react-router-bootstrap';
import { connect } from 'react-redux';
import * as actions from '../actions';
import selectors from '../selectors';

const mapStateToProps = (state) => ({
    username: selectors.username(state),
});

const mapDispatchToProps = (dispatch) => ({
    logout: () => dispatch(actions.logout()),
});

export default connect(mapStateToProps, mapDispatchToProps)(
    ({ username, logout }) => {
        let account;

        if (username) {
            account = (
                <NavDropdown title={username} id="basic-nav-dropdown">
                    <NavDropdown.Item onClick={logout}>Logout</NavDropdown.Item>
                </NavDropdown>
            );
        } else {
            account = (
                <LinkContainer to="/login">
                    <Nav.Link>Login</Nav.Link>
                </LinkContainer>
            );
        }

        return (
            <Navbar>
                <LinkContainer to="/">
                    <Navbar.Brand>Chess</Navbar.Brand>
                </LinkContainer>
                <Navbar.Toggle aria-controls="basic-navbar-nav" />
                <Navbar.Collapse id="basic-navbar-nav">
                    <Nav activeKey={window.location.pathname}>
                        {account}
                    </Nav>
                </Navbar.Collapse>
            </Navbar>
        );
    },
);
