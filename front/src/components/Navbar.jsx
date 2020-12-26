import React from 'react';
import { withRouter } from 'react-router-dom';
import Navbar from 'react-bootstrap/Navbar';
import Nav from 'react-bootstrap/Nav';
import NavDropdown from 'react-bootstrap/NavDropdown';
import { LinkContainer } from 'react-router-bootstrap';
import { connect } from 'react-redux';
import * as actions from '../actions';
import selectors from '../selectors';

const NavLink = ({ to, value, children }) => (
    <LinkContainer to={to}>
        {value ? <Nav.Link>{value}</Nav.Link> : children}
    </LinkContainer>
);

const mapStateToProps = (state) => ({
    username: selectors.username(state),
});

const mapDispatchToProps = (dispatch) => ({
    logout: () => dispatch(actions.logout()),
});

export default withRouter(connect(mapStateToProps, mapDispatchToProps)(
    ({ username, logout, location }) => {
        let account;

        if (username) {
            account = (
                <NavDropdown title={username} id="basic-nav-dropdown">
                    <NavDropdown.Item onClick={logout}>Logout</NavDropdown.Item>
                </NavDropdown>
            );
        } else {
            account = <NavLink to="/login" value="Login" />;
        }

        return (
            <Navbar>
                <NavLink to="/">
                    <Navbar.Brand>Chess</Navbar.Brand>
                </NavLink>
                <Navbar.Toggle aria-controls="basic-navbar-nav" />
                <Navbar.Collapse id="basic-navbar-nav">
                    <Nav activeKey={location.pathname}>
                        <NavLink to="/new_game" value="New Game" />
                        {account}
                    </Nav>
                </Navbar.Collapse>
            </Navbar>
        );
    },
));
